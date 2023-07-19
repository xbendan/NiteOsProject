#include <arch/x86_64/paging.hpp>
#include <siberix/core/runtimes.hpp>
#include <siberix/mm/addrspace.hpp>

class X64Runtime : public KernelExecutive {
public:
    X64Runtime();
    ~X64Runtime();

    void setup() override;
    void loadMemory() override;
    void loadDevices() override;

    GdtPackage m_gdt;
    GdtPtr     m_gdtPtr;
    IdtPtr     m_idtPtr;
};

static inline uintptr_t readMSR(uintptr_t msr) {
    uint32_t low, high;
    asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
    return ((uintptr_t)high << 32) | low;
}

static inline void writeMSR(uint32_t msr, uint64_t value) {
    uint32_t low  = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    asm volatile("wrmsr" : : "c"(msr), "a"(low), "d"(high));
}

static inline void setCpuLocal(Cpu* cpu) {
    cpu->self = cpu;
    asm volatile("wrmsr" ::"a"((u64)cpu & 0xffffffff) /*Value low*/,
                 "d"(((u64)cpu >> 32) & 0xffffffff) /*Value high*/,
                 "c"(MSR_KERN_GS_BASE) /*Set Kernel GS Base*/);
    asm volatile("wrmsr" ::"a"((u64)cpu & 0xffffffff) /*Value low*/,
                 "d"(((u64)cpu >> 32) & 0xffffffff) /*Value high*/,
                 "c"(MSR_GS_BASE) /*Set Kernel GS Base*/);
}

static inline Cpu* getCpuLocal() {
    Cpu* Cpu;
    asm volatile("swapgs; movq %%gs:0, %0; swapgs;" : "=r"(cpu));
    return cpu;
}

static inline bool checkInterrupts() {
    volatile unsigned long flags;
    asm volatile(
        "pushfq;"
        "pop %0;"
        : "=rm"(flags)::"memory", "cc");
    return (flags & 0x200) != 0;
}