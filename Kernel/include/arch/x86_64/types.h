#pragma once

#include <common/string.h>
#include <common/typedefs.h>
#include <siberix/core/runtimes.h>
#include <siberix/proc/process.h>

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

#define GDT_ENTRY_COUNT 5
#define GDT_SEGMENT 0b00010000
#define GDT_PRESENT 0b10000000
#define GDT_USER 0b01100000
#define GDT_EXECUTABLE 0b00001000
#define GDT_READWRITE 0b00000010
#define GDT_LM_GRANULARITY 0b0010
#define GDT_FLAGS 0b1100

struct GdtPtr {
    u16 limit;
    u64 base;
} __attribute__((packed));

struct GdtEntry {
    u16 limitLow;
    u16 baseLow;
    u8  baseMedium;
    u8  access;
    u8  granularity;
    u8  baseHigh;
} __attribute__((packed));

struct GdtExtraEntry {
    u32 baseHighest;
    u32 __reserved__;
} __attribute__((packed));

struct GdtPackage;

class TaskStateSegment {
public:
    u32 __reserved__0 __attribute__((aligned(0x10)));
    u64 rsp[3];
    u64 __reserved__1;
    u64 ist[7];
    u32 __reserved__2;
    u32 __reserved__3;
    u16 __reserved__4;
    u16 iopbOffset;

    void init(GdtPackage *package);
} __attribute__((packed));

struct GdtTssEntry {
    u16 len;
    u16 baseLow;
    u8  baseMedium;
    u8  flags_a;
    u8  flags_b;
    u8  baseHigh;
    u32 baseUpper;
    u32 __reserved__;

    GdtTssEntry() {}
    GdtTssEntry(TaskStateSegment &tss)
        : len(sizeof(TaskStateSegment)),
          baseLow(((u64)&tss) & 0xffff),
          baseMedium(((u64)&tss) >> 16 & 0xff),
          flags_a(0b10001001),
          flags_b(0),
          baseHigh(((u64)&tss) >> 24 & 0xff),
          baseUpper(((u64)&tss) >> 32 & 0xffffffff),
          __reserved__() {}
} __attribute__((packed));

struct GdtPackage {
    GdtEntry    entries[GDT_ENTRY_COUNT];
    GdtTssEntry tss;

    GdtPackage()
        : entries{
              GdtEntry{0x0000,  0x0000, 0x00, 0x00, 0x00,                       0x00},
              GdtEntry{ 0xFFFF, 0x0000, 0x00, 0x9A, (1 << 5) | (1 << 7) | 0x0F, 0x00},
              GdtEntry{ 0xFFFF, 0x0000, 0x00, 0x92, (1 << 5) | (1 << 7) | 0x0F, 0x00},
              GdtEntry{ 0xFFFF, 0x0000, 0x00, 0xFA, (1 << 5) | (1 << 7) | 0x0F, 0x00},
              GdtEntry{ 0xFFFF, 0x0000, 0x00, 0xF2, (1 << 5) | (1 << 7) | 0x0F, 0x00}
    },
          tss(){};
    GdtPackage(TaskStateSegment& tss)
        : entries{
              GdtEntry{0x0000,  0x0000, 0x00, 0x00, 0x00,                       0x00},
              GdtEntry{ 0xFFFF, 0x0000, 0x00, 0x9A, (1 << 5) | (1 << 7) | 0x0F, 0x00},
              GdtEntry{ 0xFFFF, 0x0000, 0x00, 0x92, (1 << 5) | (1 << 7) | 0x0F, 0x00},
              GdtEntry{ 0xFFFF, 0x0000, 0x00, 0xFA, (1 << 5) | (1 << 7) | 0x0F, 0x00},
              GdtEntry{ 0xFFFF, 0x0000, 0x00, 0xF2, (1 << 5) | (1 << 7) | 0x0F, 0x00}
    },
          tss(GdtTssEntry(tss)){};
} __attribute__((packed)) __attribute__((aligned(0x10)));

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

struct IdtPtr {
    u16 limit;
    u64 base;
} __attribute__((packed));

struct IdtEntry {
    u16 baseLow;
    u16 selector;
    u8  ist;
    u8  flags;
    u16 baseMedium;
    u32 baseHigh;
    u32 __reserved__ = 0;

    constexpr IdtEntry()
        : baseLow(0),
          selector(0),
          ist(0),
          flags(0),
          baseMedium(0),
          baseHigh(0) {}

    constexpr IdtEntry(u8 num, u64 base, u16 sel, u8 flags, u8 ist)
        : baseLow((u16)(base & 0xffff)),
          selector(sel),
          ist(ist),
          flags(flags),
          baseMedium((u16)(base >> 16 & 0xffff)),
          baseHigh((u32)(base >> 32) & 0xffffffff) {}
} __attribute__((packed));

struct RegisterContext {
    u64 r15, r14, r13, r12, r11, r10, r9, r8;
    u64 rbp, rdi, rsi, rdx, rcx, rbx, rax;
    u64 intno, err;
    u64 rip, cs, rflags, rsp, ss;
} __attribute__((packed));

struct CpuId {
    char vstr[12];
    char vend = '\0';
    u32  ecx;
    u32  edx;
};

class X64Thread : public Thread {
public:
    X64Thread();
    X64Thread(Process *process, u32 threadId);
    ~X64Thread();

    u32 esp0;
    u32 ss0;

    void *userStack;
    void *userStackBase;
    void *kernelStack;
    void *kernelStackBase;

    u64   fsBase;
    void *fxState;

    RegisterContext registers;
    RegisterContext lastSyscall;
};

struct Cpu {
    Cpu *self;
    u32  apicId;

    GdtPackage      *gdt;
    GdtPtr           gdtPtr;
    IdtPtr           idtPtr;
    TaskStateSegment tss __attribute__((aligned(0x10)));

    Thread *currentThread;
    Thread *idleThread;
};

enum ModelSpecificRegisters {
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