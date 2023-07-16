#include <arch/x86_64/arch.h>
#include <common/logger.h>
#include <common/string.h>

#include <arch/x86_64/apic.hpp>
#include <arch/x86_64/paging.hpp>
#include <arch/x86_64/smpdefines.inc>
#include <siberix/drivers/acpi/acpi_device.hpp>
#include <siberix/proc/sched.hpp>

extern void* smpTrampolineStart;
extern void* smpTrampolineEnd;

volatile uint16_t* smpMagicValue = (uint16_t*)SMP_TRAMPOLINE_DATA_START_FLAG;
volatile uint16_t* smpTrampolineCpuID = (uint16_t*)SMP_TRAMPOLINE_CPU_ID;
GdtPtr*            smpGdtPtr          = (GDTPointer*)SMP_TRAMPOLINE_GDT_PTR;
volatile uint64_t* smpRegisterCR3     = (uint64_t*)SMP_TRAMPOLINE_CR3;
volatile uint64_t* smpStack           = (uint64_t*)SMP_TRAMPOLINE_STACK;
volatile uint64_t* smpEntry2          = (uint64_t*)SMP_TRAMPOLINE_ENTRY2;
volatile bool      doneInit           = false;

void cpuTrampolineStart(u16 cpuId) {
    X64Runtime* rt  = static_cast<X64Runtime*>(runtime());
    Cpu*        cpu = rt->getScheduler().cpu(cpuId);

    setCpuLocal(cpu);

    cpu->gdt = reinterpret_cast<GdtPackage*>(rt->getMemory().alloc4KPages(1));
    memcpy(cpu->gdt, (void*)rt->m_gdtPtr.base, rt->m_gdtPtr.limit + 1);
    cpu->gdtPtr = { .limit = rt->m_gdtPtr.limit, .base = (u64)cpu->gdt };
    cpu->idtPtr = { .limit = rt->m_idtPtr.limit,
                    .base  = (u64)rt->m_idtPtr.base };

    asm volatile("lgdt (%%rax)" ::"a"(&cpu->gdtPtr));
    asm volatile("lidt %0" ::"m"(cpu->idtPtr));

    cpu->tss.init(cpu->gdt);
    ApicDevice::getLocalApicId(cpuId).setup();

    asm("sti");
    doneInit = true;

    for (;;) asm volatile("pause");
}

Scheduler::Scheduler()
    : m_kernelProcess(
          new Process("SiberixKernel", nullptr, 0, TaskTypeSystemProcess)) {
    X64Runtime* rt = static_cast<X64Runtime*>(runtime());

    m_cpus[0] = new Cpu(){ .apicId        = 0,
                           .gdt           = rt->m_gdt,
                           .gdtPtr        = rt->m_gdtPtr,
                           .idtPtr        = rt->m_idtPtr,
                           .currentThread = nullptr,
                           .idleThread    = new Thread() };
    setCpuLocal(m_cpus[0]);
    m_cpus[0]->tss.init(m_cpus[0]->gdt);

    rt->getDeviceTree()
        .enumerateDevice(DeviceTypeProcessor)
        .forEach([&](u8 processorId) -> void {
            if (processorId) {
                if (m_cpus[processorId] != nullptr) {
                    return;
                }
                Logger::getLogger("hw").info("CPU [%u] is being initialized",
                                             processorId);

                ApicLocalInterface& interface =
                    ApicDevice::getLocalApicId(processorId);
                m_cpus[processorId]  = new Cpu();
                *m_cpus[processorId] = { .self          = m_cpus[processorId],
                                         .apicId        = processorId,
                                         .currentThread = nullptr,
                                         .idleThread    = new Thread() };

                *smpMagicValue      = 0;
                *smpTrampolineCpuID = processorId;
                *smpEntry2          = (uint64_t)cpuTrampolineStart;
                *smpStack = (uint64_t)(rt->getMemory().alloc4KPages(4)) + 16384;
                *smpGdtPointer = rt->m_gdtPtr;

                asm volatile("mov %%cr3, %%rax" : "=a"(*smpRegisterCR3));

                interface.sendInterrupt(
                    processorId, ICR_DSH_DEST, ICR_MESSAGE_TYPE_INIT, 0);
                // Sleep 50 ms
                while (*smpMagicValue != 0xb33f) {
                    interface.sendInterrupt(processorId,
                                            ICR_DSH_DEST,
                                            ICR_MESSAGE_TYPE_STARTUP,
                                            (SMP_TRAMPOLINE_ENTRY >> 12));
                    // Sleep 200 ms
                }

                while (!doneInit) asm("pause");
                Logger::getLogger("hw").info("CPU [%u] loaded", processorId);
                doneInit = false;
            }
        });

    Logger::getLogger("hw").success("SMP initialized.");
}

Scheduler::switchTask(Thread* newThread) {
    Cpu* cpu = getCpuLocal();

    asm volatile("fxrstor64 (%0)" ::"r"((uintptr_t)newThread->fxState)
                 : "memory");
    writeMSR(0xc0000100 /* Fs Base */, newThread->fsBase);
    Process* process = newThread->m_parent;

    cpu->currentThread = newThread;
    cpu->tss.rsp[0]    = reinterpret_cast<u64>(newThread->m_kernelStack);

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
            iretq)" ::"r"(&newThread->m_registers),
        "r"(reinterpret_cast<Paging::X64AddressSpace>(process->m_addressSpace)
                ->pml4Phys));
}