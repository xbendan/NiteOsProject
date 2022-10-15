#include <x86_64/smp.h>
#include <x86_64/cpu.h>
#include <x86_64/acpi.h>
#include <mm/malloc.h>

extern struct GlobalDescTablePointer g_Gdtr;
extern struct InterruptDescTablePointer g_Idtr;

struct CPUCore *cpus[MAX_CPU_AMOUNT];

void __smp_SetTrampoline()
{
    
}

void __smp_InitializeProcessor(uint16_t cpuId)
{
    cpus[cpuId] = (struct CPUCore *) KernelAllocateObject(sizeof(struct CPUCore));
    
    cpus[cpuId]->id = cpuId;
    
}

void SMP_Initialize()
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
            __smp_InitializeProcessor(g_Processors[i]);
    }
}