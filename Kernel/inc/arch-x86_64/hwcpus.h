#include <siberix/hwtypes.h>
#include <stdcxx/types.h>

namespace Kern::Platform::X64 {
    enum MsrEnum
    {
        MSR_APIC              = 0x1B,
        MSR_EFER              = 0xC0000080,
        MSR_STAR              = 0xC0000081,
        MSR_LSTAR             = 0xC0000082,
        MSR_COMPAT_STAR       = 0xC0000083,
        MSR_SYSCALL_FLAG_MASK = 0xC0000084,
        MSR_FS_BASE           = 0xC0000100,
        MSR_GS_BASE           = 0xC0000101,
        MSR_KERN_GS_BASE      = 0xc0000102,
    };

    static inline uint64_t rdmsr(uint32_t reg)
    {
        uint32_t low, high;
        asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(reg));
        return ((uint64_t)high << 32) | low;
    }

    static inline void wrmsr(uint32_t reg, uint64_t value)
    {
        uint32_t low  = value & 0xFFFFFFFF;
        uint32_t high = value >> 32;
        asm volatile("wrmsr" : : "c"(reg), "a"(low), "d"(high));
    }

    static inline void swapgs()
    {
        asm volatile("swapgs");
    }

    static inline void setCPULocal(Kern::Hal::Cpu* cpu)
    {
        cpu->m_self = cpu;
        asm volatile("wrmsr" ::"a"((uint64_t)cpu & 0xffffffff) /*Value low*/,
                     "d"(((uint64_t)cpu >> 32) & 0xffffffff) /*Value high*/,
                     "c"(MSR_KERN_GS_BASE) /*Set Kernel GS Base*/);
        asm volatile("wrmsr" ::"a"((uint64_t)cpu & 0xffffffff) /*Value low*/,
                     "d"(((uint64_t)cpu >> 32) & 0xffffffff) /*Value high*/,
                     "c"(MSR_GS_BASE) /*Set Kernel GS Base*/);
    }

    static inline Kern::Hal::Cpu* getCPULocal()
    {
        Kern::Hal::Cpu* cpu;
        asm volatile("swapgs; movq %%gs:0, %0; swapgs;" : "=r"(cpu));
        return cpu;
    }
}