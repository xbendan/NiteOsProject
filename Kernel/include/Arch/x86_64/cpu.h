#pragma once

#include <Arch/x86_64/gdt.h>
#include <Arch/x86_64/idt.h>

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

#define CPU_CORE_ID ThisCPU()
#define MAX_CPU_AMOUNT 64

#define GDT_ENTRY_COUNT 5
#define GDT_SEGMENT 0b00010000
#define GDT_PRESENT 0b10000000
#define GDT_USER 0b01100000
#define GDT_EXECUTABLE 0b00001000
#define GDT_READWRITE 0b00000010
#define GDT_LM_GRANULARITY 0b0010
#define GDT_FLAGS 0b1100

struct GlobalDescTablePointer
{
    uint16_t size;
    uint64_t base;
} __attribute__((packed));

struct GlobalDescTableEntry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_medium;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

typedef struct TaskStateSegment
{
    uint32_t ign_0 __attribute__((aligned(16)));
    uint64_t rsp[3];
    uint64_t ign_1;
    uint64_t ist[7];
    uint32_t ign_2;
    uint32_t ign_3;
    uint16_t ign_4;
    uint16_t iopb_offset;
} __attribute__((packed)) tss_t;

struct GlobalDescTableTaskEntry
{
    uint16_t len;
    uint16_t base_low;
    uint8_t base_medium;
    uint8_t flags_a;
    uint8_t flags_b;
    uint8_t base_high;
    uint32_t base_upper;
    uint32_t ign;
} __attribute__((packed));

struct GlobalDescTablePackage
{
    struct GlobalDescTableEntry entries[GDT_ENTRY_COUNT];
    struct GlobalDescTableTaskEntry tss;
} __attribute__((packed));

#define IDT_DIVIDE_BY_ZERO 0x00
#define IDT_SINGLE_STEP 0x01
#define IDT_NMI 0x02
#define IDT_BREAKPOINT 0x03
#define IDT_OVERFLOW 0x04
#define IDT_BOUND_RANGE_EXCEEDED 0x05
#define IDT_INVALID_OPCODE 0x06
#define IDT_COPROCESSOR_NOT_AVAILABLE 0x07
#define IDT_DOUBLE_FAULT 0x08

#define IDT_INVALID_TASK_STATE_SEGMENT 0x0A
#define IDT_SEGMENT_NOT_PRESENT 0x0B
#define IDT_STACK_SEGMENT_FAULT 0x0C
#define IDT_GENERAL_PROTECTION_FAULT 0x0D
#define IDT_PAGE_FAULT 0x0E

#define IDT_X87_FLOATING_POINT_EXCEPTION 0x10
#define IDT_ALIGNMENT_CHECK 0x11
#define IDT_MACHINE_CHECK 0x12
#define IDT_SIMD_FLOATING_POINT_EXCEPTION 0x13
#define IDT_VIRTUALIZATION_EXCEPTION 0x14
#define IDT_CONTROL_PROTECTION_EXCEPTION 0x15

#define IDT_FLAGS_INTGATE 0x8E
#define IDT_FLAGS_TRAPGATE 0xEF
#define IDT_FLAGS_USER 0b01100000

#define IDT_ENTRY_COUNT 256

struct InterruptDescTablePointer
{
    uint16_t size;
    uint64_t base;
} __attribute__((packed));

struct InterruptDescTableEntry
{
    uint16_t base_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t flags;
    uint16_t base_med;
    uint32_t base_high;
    uint32_t ign;
} __attribute__((packed));

typedef struct RegisterContext
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t intno, err;
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed)) registers_t;

typedef struct CPUIDInfo
{
    char vendor_str[12];
    char vendor_end = '\0';

    uint32_t ecx, edx;
} cpuid_info_t;

typedef struct ProcessorCore
{
    struct ProcessorCore *self;
    uint64_t id;
    
    void *gdt;
    struct GlobalDescTablePointer gdtPtr;
    struct InterruptDescTablePointer idtPtr;

    tss_t tss __attribute__((aligned(16)));

    struct Thread *currentThread;
    struct Thread *idleThread;
    struct Process *idleProcess;
} processor_t;

static inline uintptr_t ReadMsr(
    uintptr_t       msr
) {
    uint32_t low, high;
    asm volatile("rdmsr"
                 : "=a"(low), "=d"(high)
                 : "c"(msr));
    return ((uintptr_t) high << 32) | low;
}

static inline void WriteMsr(
    uint32_t        msr, 
    uint64_t        value
) {
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    asm volatile("wrmsr"
                 :
                 : "c"(msr), "a"(low), "d"(high));
}

static inline bool isIntEnabled() {
    uint64_t flags;
    asm volatile("pushf\n\t"
                 "pop %o"
                 : "=g"(flags));
    return flags & (1 << 9);
}

enum ModelSpecificRegisters {
    MSR_APIC = 0x1B,
    MSR_EFER = 0xC0000080,
    MSR_STAR = 0xC0000081,
    MSR_LSTAR = 0xC0000082,
    MSR_COMPAT_STAR = 0xC0000083,
    MSR_SYSCALL_FLAG_MASK = 0xC0000084,
    MSR_FS_BASE = 0xC0000100,
    MSR_GS_BASE = 0xC0000101,
    MSR_KERN_GS_BASE = 0xc0000102,
};

CPUIDInfo CPUID();
void SetCPULocal(processor_t *cpu);
processor_t *GetCPULocal();

int ThisCPU();

namespace TSS {
    void Initialize(tss_t* tss, void* gdt);

    inline void SetKernelStack(tss_t* tss, uint64_t stack){
        tss->rsp[0] = stack;
    }
}