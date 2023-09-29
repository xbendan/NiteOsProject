#pragma once

#include <arch/x86_64/interrupts.h>
#include <arch/x86_64/paging.h>
#include <arch/x86_64/types.h>
#include <siberix/core/runtimes.h>
#include <siberix/mm/addrspace.h>

class SbrxkrnlX64Impl : public SiberixKernel
{
public:
    SbrxkrnlX64Impl();
    ~SbrxkrnlX64Impl();

    bool setupArch();

    utils::Optional<Interrupt> getInterrupt(unsigned index);

    GdtPackage m_gdt;
    GdtPtr     m_gdtPtr;
    IdtPtr     m_idtPtr;
    Cpu*       m_cpus[256];
};

static inline u64
readMSR(u64 msr)
{
    u32 low, high;
    asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
    return ((u64)high << 32) | low;
}

static inline void
writeMSR(u32 msr, u64 value)
{
    u32 low  = value & 0xFFFFFFFF;
    u32 high = value >> 32;
    asm volatile("wrmsr" : : "c"(msr), "a"(low), "d"(high));
}

static inline void
setCpuLocal(Cpu* cpu)
{
    cpu->self = cpu;
    asm volatile("wrmsr" ::"a"((u64)cpu & 0xffffffff) /*Value low*/,
                 "d"(((u64)cpu >> 32) & 0xffffffff) /*Value high*/,
                 "c"(MSR_KERN_GS_BASE) /*Set Kernel GS Base*/);
    asm volatile("wrmsr" ::"a"((u64)cpu & 0xffffffff) /*Value low*/,
                 "d"(((u64)cpu >> 32) & 0xffffffff) /*Value high*/,
                 "c"(MSR_GS_BASE) /*Set Kernel GS Base*/);
}

static inline Cpu*
getCpuLocal()
{
    Cpu* cpu;
    asm volatile("swapgs; movq %%gs:0, %0; swapgs;" : "=r"(cpu));
    return cpu;
}

static inline bool
checkInterrupts()
{
    volatile unsigned long flags;
    asm volatile("pushfq;"
                 "pop %0;"
                 : "=rm"(flags)::"memory", "cc");
    return (flags & 0x200) != 0;
}