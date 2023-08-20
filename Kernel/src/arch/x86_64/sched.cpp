#include <arch/x86_64/apic.h>
#include <arch/x86_64/arch.h>
#include <arch/x86_64/paging.h>
#include <common/logger.h>
#include <common/string.h>
#include <siberix/drivers/acpi/acpi_device.h>
#include <siberix/proc/sched.h>

#include <arch/x86_64/smpdefines.inc>

extern void* smpTrampolineStart;
extern void* smpTrampolineEnd;

volatile u16* smpMagicValue      = (u16*)SMP_TRAMPOLINE_DATA_START_FLAG;
volatile u16* smpTrampolineCpuID = (u16*)SMP_TRAMPOLINE_CPU_ID;
GdtPtr*       smpGdtPtr          = (GdtPtr*)SMP_TRAMPOLINE_GDT_PTR;
volatile u64* smpRegisterCR3     = (u64*)SMP_TRAMPOLINE_CR3;
volatile u64* smpStack           = (u64*)SMP_TRAMPOLINE_STACK;
volatile u64* smpEntry2          = (u64*)SMP_TRAMPOLINE_ENTRY2;
volatile bool doneInit           = false;

void trampolineStart(u16 cpuId) {
    SbrxkrnlX64Impl* rt  = static_cast<SbrxkrnlX64Impl*>(siberix());
    Cpu*          cpu = siberix()->getScheduler().cpu(cpuId);

    setCpuLocal(cpu);

    cpu->gdt = reinterpret_cast<GdtPackage*>(rt->getMemory().alloc4KPages(1));
    memcpy(cpu->gdt, (void*)rt->m_gdtPtr.base, rt->m_gdtPtr.limit + 1);
    cpu->gdtPtr = { .limit = rt->m_gdtPtr.limit, .base = (u64)cpu->gdt };
    cpu->idtPtr = { .limit = rt->m_idtPtr.limit, .base = (u64)rt->m_idtPtr.base };

    asm volatile("lgdt (%%rax)" ::"a"(&cpu->gdtPtr));
    asm volatile("lidt %0" ::"m"(cpu->idtPtr));

    cpu->tss.init(cpu->gdt);
    ApicDevice::getInterface(cpuId).setup();

    asm("sti");
    doneInit = true;

    for (;;) asm volatile("pause");
}

Scheduler::Scheduler()
    : m_kernelProcess(new Process("SiberixKernel", nullptr, 0, TaskType::System)) {
    SbrxkrnlX64Impl* e = static_cast<SbrxkrnlX64Impl*>(siberix());

    m_cpus[0] = new Cpu{ .apicId        = 0,
                         .gdt           = &(e->m_gdt),
                         .gdtPtr        = e->m_gdtPtr,
                         .idtPtr        = e->m_idtPtr,
                         .currentThread = nullptr,
                         .idleThread    = new Thread() };
    setCpuLocal(m_cpus[0]);
    m_cpus[0]->tss.init(m_cpus[0]->gdt);

    siberix()
        ->getConnectivity()
        ->enumerateDevice(DeviceType::Processor)
        .forEach([&](u8 processorId) -> void {
            if (processorId) {
                if (m_cpus[processorId] != nullptr) {
                    return;
                }
                Logger::getLogger("hw").info("CPU [%u] is being initialized", processorId);

                ApicLocalInterface& interface = ApicDevice::getInterface(processorId);
                m_cpus[processorId]           = new Cpu();
                *m_cpus[processorId]          = { .self          = m_cpus[processorId],
                                                  .apicId        = processorId,
                                                  .currentThread = nullptr,
                                                  .idleThread    = new X64Thread() };

                *smpMagicValue      = 0;
                *smpTrampolineCpuID = processorId;
                *smpEntry2          = (u64)trampolineStart;
                *smpStack           = (u64)(e->getMemory().alloc4KPages(4)) + 16384;
                *smpGdtPtr          = e->m_gdtPtr;

                asm volatile("mov %%cr3, %%rax" : "=a"(*smpRegisterCR3));

                interface.sendInterrupt(ICR_DSH_DEST, ICR_MESSAGE_TYPE_INIT, 0);
                // Sleep 50 ms
                while (*smpMagicValue != 0xb33f) {
                    interface.sendInterrupt(
                        ICR_DSH_DEST, ICR_MESSAGE_TYPE_STARTUP, (SMP_TRAMPOLINE_ENTRY >> 12));
                    // Sleep 200 ms
                }

                while (!doneInit) asm("pause");
                Logger::getLogger("hw").info("CPU [%u] loaded", processorId);
                doneInit = false;
            }
        });

    Logger::getLogger("hw").success("SMP initialized.");
}

bool Scheduler::switchThread(Thread* t) {
    Cpu*       cpu    = getCpuLocal();
    X64Thread* thread = static_cast<X64Thread*>(t);

    asm volatile("fxrstor64 (%0)" ::"r"((u64)thread->fxState) : "memory");
    writeMSR(0xc0000100 /* Fs Base */, thread->fsBase);
    Process* process = thread->m_parent;

    cpu->currentThread = thread;
    cpu->tss.rsp[0]    = reinterpret_cast<u64>(thread->kernelStack);

    asm volatile(
        R"(mov %0, %%rsp;
            mov %1, %%rax;
            pop %%r15;
            pop %%r14;
            pop %%r13;
            pop %%r12;
            pop %%r11;
            pop %%r10;
            pop %%r9;
            pop %%r8;
            pop %%rbp;
            pop %%rdi;
            pop %%rsi;
            pop %%rdx;
            pop %%rcx;
            pop %%rbx;
            
            mov %%rax, %%cr3
            pop %%rax
            addq $8, %%rsp
            iretq)" ::"r"(&thread->registers),
        "r"(reinterpret_cast<Paging::X64AddressSpace*>(process->getAddressSpace())->pml4Phys));
}