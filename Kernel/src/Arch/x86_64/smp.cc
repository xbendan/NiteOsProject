#include <Arch/x86_64/smp.h>
#include <Arch/x86_64/cpu.h>
#include <Arch/x86_64/acpi.h>
#include <mm/mem.h>
#include <mm/kmalloc.h>

#include "Arch/x86_64/smpdefines.inc"

extern struct GlobalDescTablePointer g_Gdtr;
extern struct InterruptDescTablePointer g_Idtr;
struct CPUCore *cpus[MAX_CPU_AMOUNT];

namespace SMP
{
    void SetTrampoline()
    {

    }

    void SetProcessor(uint16_t cpuId)
    {
        cpus[cpuId] = (struct CPUCore *) kmalloc(sizeof(struct CPUCore));
        cpus[cpuId]->id = cpuId;
    }

    void Initialize()
    {
        cpus[0] = (cpu_info_t *) kmalloc(sizeof(cpu_info_t));
        
        *cpus[0] = (cpu_info_t)
        {
            .id = 0,
            .gdt = (void *) g_Gdtr.base,
            .gdtPtr = g_Gdtr,
            .idtPtr = g_Idtr,
            .currentThread = NULL,
            .idleThread = NULL
        };

        SetCPULocal(cpus[0]);

        for(int i = 0; i < ACPI::g_ProcessorCount; i++)
        {
            if(ACPI::g_Processors[i] != 0)
                SetProcessor(ACPI::g_Processors[i]);
        }

        for (;;) asm("hlt");
    }
} // namespace SMP