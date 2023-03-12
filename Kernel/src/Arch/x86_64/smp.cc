#include <Arch/x86_64/smp.h>
#include <Arch/x86_64/cpu.h>
#include <Arch/x86_64/acpi.h>
#include <Arch/x86_64/apic.h>
#include <Mem/Memory.h>
#include <Mem/Page.h>
#include <Mem/KMemAlloc.h>
#include <driver/video.h>
#include <timer.h>
#include <system.h>

#include "Arch/x86_64/smpdefines.inc"

extern void *SMPTrampolineStart;
extern void *SMPTrampolineEnd;

extern struct GlobalDescTablePointer g_Gdtr;
extern struct InterruptDescTablePointer g_Idtr;
volatile bool doneInit = false;
processor_t *cpus[MAX_CPU_AMOUNT];

namespace SMP
{
    volatile uint16_t* smpMagicValue = (uint16_t*) SMP_MAGIC;
    volatile uint16_t* smpTrampolineCpuID = (uint16_t*) SMP_TRAMPOLINE_CPU_ID;
    GlobalDescTablePointer* smpGdtPointer = (GlobalDescTablePointer *) SMP_TRAMPOLINE_GDT_PTR;
    volatile uint64_t* smpRegisterCR3 = (uint64_t*)SMP_TRAMPOLINE_CR3;
    volatile uint64_t* smpStack = (uint64_t*)SMP_TRAMPOLINE_STACK;
    volatile uint64_t* smpEntry2 = (uint64_t*)SMP_TRAMPOLINE_ENTRY2;

    void TrampolineEntry(uint16_t cpuId)
    {
        processor_t *cpu = cpus[cpuId];
        cpu->currentThread = nullptr;

        SetCPULocal(cpu);

        cpu->gdt = reinterpret_cast<void *>(Memory::KernelAllocate4KPages(1));
        memcpy(cpu->gdt, (void *) g_Gdtr.base, g_Gdtr.size + 1);
        cpu->gdtPtr = {
            .size = g_Gdtr.size,
            .base = (uint64_t) cpu->gdt
        };
        cpu->idtPtr = {
            .size = g_Idtr.size,
            .base = g_Idtr.base
        };

        asm volatile("lgdt (%%rax)" ::"a"(&cpu->gdtPtr));
        asm volatile("lidt %0" :: "m"(cpu->idtPtr));

        TSS::Initialize(&cpu->tss, cpu->gdt);
        APIC::Local::Enable();

        doneInit = true;

        asm("sti");
        Video::WriteText("Processor initialized.");

        for (;;)
            asm volatile("pause");
    }

    void InitializeProcessor(uint16_t cpuId)
    {
        cpus[cpuId] = (processor_t *) kmalloc(sizeof(processor_t));

        *cpus[cpuId] = {
            .self = cpus[cpuId],
            .id = cpuId,
        };

        *smpMagicValue = 0;
        *smpTrampolineCpuID = cpuId;
        *smpEntry2 = (uint64_t) TrampolineEntry;
        *smpStack = (uint64_t) Memory::KernelAllocate4KPages(4) + 16384;
        *smpGdtPointer = g_Gdtr;

        asm volatile("mov %%cr3, %%rax" : "=a"(*smpRegisterCR3));

        Video::WriteText("APIC signal sending");

        APIC::Local::SendIPI(ACPI::g_Processors[cpuId], ICR_DSH_DEST, ICR_MESSAGE_TYPE_INIT, 0);
        // APIC::Local::SendIPI(ACPI::g_Processors[cpuId], 0x4500);
        ACPI::Timer::Sleep(50000);

        while (*smpMagicValue != 0xB33F) {
            // APIC::Local::SendIPI(ACPI::g_Processors[cpuId], 0x4601);
            APIC::Local::SendIPI(ACPI::g_Processors[cpuId], ICR_DSH_DEST, ICR_MESSAGE_TYPE_STARTUP, (SMP_TRAMPOLINE_ENTRY >> 12));
            ACPI::Timer::Sleep(2000000);
        }
        Video::WriteText("APIC signal sent.");
        

        while (!doneInit)
            asm("pause");

        doneInit = false;
    }

    void Initialize()
    {
        cpus[0] = (processor_t *) kmalloc(sizeof(processor_t));
        
        *cpus[0] = (processor_t)
        {
            .id = 0,
            .gdt = (void *) g_Gdtr.base,
            .gdtPtr = g_Gdtr,
            .idtPtr = g_Idtr,
            .currentThread = nullptr,
            .idleThread = nullptr
        };
        SetCPULocal(cpus[0]);

        memcpy((void *) SMP_TRAMPOLINE_ENTRY, &SMPTrampolineStart, ((uint64_t) &SMPTrampolineEnd) - ((uint64_t) &SMPTrampolineStart));
        
        System::Out("%u", ACPI::g_Processors[1]);
        for(int i = 0; i < ACPI::g_ProcessorCount; i++)
        {
            if(ACPI::g_Processors[i] != 0 && i != ThisCPU()) {
                System::Out("Initializing CPU %u", ACPI::g_Processors[i]);
                InitializeProcessor(ACPI::g_Processors[i]);
            }
        }

        Video::WriteText("SMP initialized.");
        for (;;) asm("hlt");
    }
} // namespace SMP