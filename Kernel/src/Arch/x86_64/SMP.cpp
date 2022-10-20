#include <Arch/x86_64/SMP.h>
#include <Arch/x86_64/CPU.h>
#include <Arch/x86_64/ACPI.h>
#include <Memory/Memory.h>

#include "smpdefines.inc"

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
        cpus[cpuId] = (struct CPUCore *) KernelAllocateObject(sizeof(struct CPUCore));
        cpus[cpuId]->id = cpuId;
    }

    void Initialize()
    {
        cpus[0] = (cpu_info_t *) KernelAllocateObject(sizeof(cpu_info_t));

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

        for(int i = 0; i < g_ProcessorCount; i++)
        {
            if(g_Processors[i] != 0)
                SetProcessor(g_Processors[i]);
        }
    }
} // namespace SMP