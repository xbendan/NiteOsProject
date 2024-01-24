#include <siberix/hwtypes.h>
#include <stdcxx/types.h>

#define CPUID_ECX_SSE3 (1 << 0)
#define CPUID_ECX_PCLMUL (1 << 1)
#define CPUID_ECX_DTES64 (1 << 2)
#define CPUID_ECX_MONITOR (1 << 3)
#define CPUID_ECX_DS_CPL (1 << 4)
#define CPUID_ECX_VMX (1 << 5)
#define CPUID_ECX_SMX (1 << 6)
#define CPUID_ECX_EST (1 << 7)
#define CPUID_ECX_TM2 (1 << 8)
#define CPUID_ECX_SSSE3 (1 << 9)
#define CPUID_ECX_CID (1 << 10)
#define CPUID_ECX_FMA (1 << 12)
#define CPUID_ECX_CX16 (1 << 13)
#define CPUID_ECX_ETPRD (1 << 14)
#define CPUID_ECX_PDCM (1 << 15)
#define CPUID_ECX_PCIDE (1 << 17)
#define CPUID_ECX_DCA (1 << 18)
#define CPUID_ECX_SSE4_1 (1 << 19)
#define CPUID_ECX_SSE4_2 (1 << 20)
#define CPUID_ECX_x2APIC (1 << 21)
#define CPUID_ECX_MOVBE (1 << 22)
#define CPUID_ECX_POPCNT (1 << 23)
#define CPUID_ECX_AES (1 << 25)
#define CPUID_ECX_XSAVE (1 << 26)
#define CPUID_ECX_OSXSAVE (1 << 27)
#define CPUID_ECX_AVX (1 << 28)

#define CPUID_EDX_FPU (1 << 0)
#define CPUID_EDX_VME (1 << 1)
#define CPUID_EDX_DE (1 << 2)
#define CPUID_EDX_PSE (1 << 3)
#define CPUID_EDX_TSC (1 << 4)
#define CPUID_EDX_MSR (1 << 5)
#define CPUID_EDX_PAE (1 << 6)
#define CPUID_EDX_MCE (1 << 7)
#define CPUID_EDX_CX8 (1 << 8)
#define CPUID_EDX_APIC (1 << 9)
#define CPUID_EDX_SEP (1 << 11)
#define CPUID_EDX_MTRR (1 << 12)
#define CPUID_EDX_PGE (1 << 13)
#define CPUID_EDX_MCA (1 << 14)
#define CPUID_EDX_CMOV (1 << 15)
#define CPUID_EDX_PAT (1 << 16)
#define CPUID_EDX_PSE36 (1 << 17)
#define CPUID_EDX_PSN (1 << 18)
#define CPUID_EDX_CLF (1 << 19)
#define CPUID_EDX_DTES (1 << 21)
#define CPUID_EDX_ACPI (1 << 22)
#define CPUID_EDX_MMX (1 << 23)
#define CPUID_EDX_FXSR (1 << 24)
#define CPUID_EDX_SSE (1 << 25)
#define CPUID_EDX_SSE2 (1 << 26)
#define CPUID_EDX_SS (1 << 27)
#define CPUID_EDX_HTT (1 << 28)
#define CPUID_EDX_TM1 (1 << 29)
#define CPUID_EDX_IA64 (1 << 30)
#define CPUID_EDX_PBE (1 << 31)

namespace Kern::Platform::X64 {
    struct CpuId
    {
        Array<char, 12> vendor;
        uint8_t         __end = '\0';
        uint32_t        ecx, edx;
    };

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

    static inline void setCPULocal(Kern::Hal::CPU* cpu)
    {
        asm volatile("wrmsr" ::"a"((uint64_t)cpu & 0xffffffff) /*Value low*/,
                     "d"(((uint64_t)cpu >> 32) & 0xffffffff) /*Value high*/,
                     "c"(MSR_KERN_GS_BASE) /*Set Kernel GS Base*/);
        asm volatile("wrmsr" ::"a"((uint64_t)cpu & 0xffffffff) /*Value low*/,
                     "d"(((uint64_t)cpu >> 32) & 0xffffffff) /*Value high*/,
                     "c"(MSR_GS_BASE) /*Set Kernel GS Base*/);
    }

    static inline Kern::Hal::CPU* getCPULocal()
    {
        Kern::Hal::CPU* cpu;
        asm volatile("swapgs; movq %%gs:0, %0; swapgs;" : "=r"(cpu));
        return cpu;
    }

    static inline CpuId cpuid()
    {
        CpuId    id;
        uint32_t regs[3]; // ebx, ecx, edx

        asm volatile("cpuid"
                     : "=b"(regs[0]), "=d"(regs[1]), "=c"(regs[2])
                     : "a"(0));
        for (int i = 0; i < 12; i)
            id.vendor[i] = regs[i / 4] >> (i % 4 * 8) & 0xFF;

        asm volatile("cpuid"
                     : "=d"(regs[2]), "=c"(regs[1])
                     : "a"(1)); // Read feature flags
        id.ecx = regs[1];
        id.edx = regs[2];

        return id;
    }
}