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

    static inline UInt64 rdmsr(UInt32 reg)
    {
        UInt32 low, high;
        asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(reg));
        return ((UInt64)high << 32) | low;
    }

    static inline Void wrmsr(UInt32 reg, UInt64 value)
    {
        UInt32 low  = value & 0xFFFFFFFF;
        UInt32 high = value >> 32;
        asm volatile("wrmsr" : : "c"(reg), "a"(low), "d"(high));
    }

    static inline Void swapgs()
    {
        asm volatile("swapgs");
    }

    static inline Void setCPULocal(Kern::Hal::Cpu* cpu)
    {
        cpu->m_self = cpu;
        asm volatile("wrmsr" ::"a"((UInt64)cpu & 0xffffffff) /*Value low*/,
                     "d"(((UInt64)cpu >> 32) & 0xffffffff) /*Value high*/,
                     "c"(MSR_KERN_GS_BASE) /*Set Kernel GS Base*/);
        asm volatile("wrmsr" ::"a"((UInt64)cpu & 0xffffffff) /*Value low*/,
                     "d"(((UInt64)cpu >> 32) & 0xffffffff) /*Value high*/,
                     "c"(MSR_GS_BASE) /*Set Kernel GS Base*/);
    }

    static inline Kern::Hal::Cpu* getCPULocal()
    {
        Kern::Hal::Cpu* cpu;
        asm volatile("swapgs; movq %%gs:0, %0; swapgs;" : "=r"(cpu));
        return cpu;
    }
}