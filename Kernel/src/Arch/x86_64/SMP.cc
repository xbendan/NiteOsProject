#include <Drivers/ACPI.h>
#include <Drivers/APIC.h>
#include <Mem/Memory.h>
#include <Mem/Page.h>
#include <Mem/KMemAlloc.h>
#include <System.h>

#include "Arch/x86_64/smpdefines.inc"
#include <Arch/x86_64/smp.h>
#include <Arch/x86_64/cpu.h>

#include <Utils/ArrayList.h>

extern void *SMPTrampolineStart;
extern void *SMPTrampolineEnd;

extern struct GDTPointer g_GDTPointer;
extern struct InterruptDescTablePointer g_IDTPointer;
volatile bool doneInit = false;
CPU *cpus[MAX_CPU_AMOUNT];

namespace SMP
{
    volatile uint16_t* smpMagicValue = (uint16_t*) SMP_MAGIC;
    volatile uint16_t* smpTrampolineCpuID = (uint16_t*) SMP_TRAMPOLINE_CPU_ID;
    GDTPointer* smpGdtPointer = (GDTPointer *) SMP_TRAMPOLINE_GDT_PTR;
    volatile uint64_t* smpRegisterCR3 = (uint64_t*)SMP_TRAMPOLINE_CR3;
    volatile uint64_t* smpStack = (uint64_t*)SMP_TRAMPOLINE_STACK;
    volatile uint64_t* smpEntry2 = (uint64_t*)SMP_TRAMPOLINE_ENTRY2;

    void TrampolineEntry(uint16_t cpuId)
    {
        CPU *cpu = cpus[cpuId];
        cpu->currentThread = nullptr;

        SetCPULocal(cpu);

        cpu->gdt = reinterpret_cast<void *>(Memory::KernelAllocate4KPages(1));
        memcpy(cpu->gdt, (void *) g_GDTPointer.base, g_GDTPointer.size + 1);
        cpu->gdtPtr = { .size = g_GDTPointer.size, .base = (uint64_t) cpu->gdt };
        cpu->idtPtr = { .size = g_IDTPointer.size, .base = g_IDTPointer.base };

        asm volatile("lgdt (%%rax)" ::"a"(&cpu->gdtPtr));
        asm volatile("lidt %0" :: "m"(cpu->idtPtr));

        TSS::Initialize(&cpu->tss, cpu->gdt);
        APIC::Local::Enable();

        doneInit = true;

        asm("sti");
        System::Out("Processor initialized.");

        for (;;)
            asm volatile("pause");
    }

    void InitializeProcessor(uint16_t cpuId)
    {
        cpus[cpuId] = (CPU *) kmalloc(sizeof(CPU));

        *cpus[cpuId] = {
            .self = cpus[cpuId],
            .id = cpuId,
        };

        *smpMagicValue = 0;
        *smpTrampolineCpuID = cpuId;
        *smpEntry2 = (uint64_t) TrampolineEntry;
        *smpStack = (uint64_t) Memory::KernelAllocate4KPages(4) + 16384;
        *smpGdtPointer = g_GDTPointer;

        asm volatile("mov %%cr3, %%rax" : "=a"(*smpRegisterCR3));

        System::Out("APIC signal sending");

        APIC::Local::SendIPI(cpuId, ICR_DSH_DEST, ICR_MESSAGE_TYPE_INIT, 0);
        g_Timers[TimerACPI]->Sleep(50);

        while (*smpMagicValue != 0xB33F) {
            APIC::Local::SendIPI(cpuId, ICR_DSH_DEST, ICR_MESSAGE_TYPE_STARTUP, (SMP_TRAMPOLINE_ENTRY >> 12));
            g_Timers[TimerACPI]->Sleep(200);
        }
        System::Out("APIC signal sent.");

        while (!doneInit)
            asm("pause");

        doneInit = false;
    }

    void Initialize()
    {
        System::Out("Initializing SMP, %u processor(s) found.", ACPI::g_Processors.Length());
        cpus[0] = (CPU *) kmalloc(sizeof(CPU));
        
        *cpus[0] = (CPU)
        {
            .id = 0,
            .gdt = (void *) g_GDTPointer.base,
            .gdtPtr = g_GDTPointer,
            .idtPtr = g_IDTPointer,
            .tss = TaskStateSegment
            .currentThread = nullptr,
            .idleThread = nullptr
        };
        SetCPULocal(cpus[0]);
        TSS::Initialize(&cpus[0]->tss, cpus[0]->gdt);

        System::Halt();

        memcpy((void *) SMP_TRAMPOLINE_ENTRY, &SMPTrampolineStart, ((uint64_t) &SMPTrampolineEnd) - ((uint64_t) &SMPTrampolineStart));

        ACPI::g_Processors.ForEach([&](uint8_t& processorId, int index) -> void {
            if (processorId)
            {
                System::Out("Initializing CPU %u", processorId);
                InitializeProcessor(processorId);
            }
        });

        System::Out("SMP initialized.");
    }
} // namespace SMP