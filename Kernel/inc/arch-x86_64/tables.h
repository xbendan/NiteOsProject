#include <stdcxx/types.h>

namespace Kern::Platform::X64 {

#define GDT_ENTRY_COUNT 5
#define GDT_SEGMENT 0b00010000
#define GDT_PRESENT 0b10000000
#define GDT_USER 0b01100000
#define GDT_EXECUTABLE 0b00001000
#define GDT_READWRITE 0b00000010
#define GDT_LM_GRANULARITY 0b0010
#define GDT_FLAGS 0b1100

    // clang-format off

    struct GdtPtr {
        UInt16 limit;
        UInt64 base;
    } __attribute__((packed));

    struct GdtEntry {
        UInt16 limitLow;
        UInt16 baseLow;
        UInt8  baseMedium;
        UInt8  access;
        UInt8  granularity;
        UInt8  baseHigh;
    } __attribute__((packed));

    struct GdtExtraEntry {
        UInt32 baseHighest;
        UInt32 __reserved__;
    } __attribute__((packed));

    struct GdtPackage;

    class TaskStateSegment {
    public:
        UInt32 __reserved__0 __attribute__((aligned(0x10)));
        UInt64 rsp[3];
        UInt64 __reserved__1;
        UInt64 ist[7];
        UInt32 __reserved__2;
        UInt32 __reserved__3;
        UInt16 __reserved__4;
        UInt16 iopbOffset;

        void init(GdtPackage *package);
    } __attribute__((packed));

    struct GdtTssEntry {
        UInt16 len;
        UInt16 baseLow;
        UInt8  baseMedium;
        UInt8  flags_a;
        UInt8  flags_b;
        UInt8  baseHigh;
        UInt32 baseUpper;
        UInt32 __reserved__;

        GdtTssEntry() {}
        GdtTssEntry(TaskStateSegment &tss)
            : len(sizeof(TaskStateSegment)),
            baseLow(((UInt64)&tss) & 0xffff),
            baseMedium(((UInt64)&tss) >> 16 & 0xff),
            flags_a(0b10001001),
            flags_b(0),
            baseHigh(((UInt64)&tss) >> 24 & 0xff),
            baseUpper(((UInt64)&tss) >> 32 & 0xffffffff),
            __reserved__() {}
    } __attribute__((packed));

    struct GdtPackage {
        GdtEntry    entries[6];
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

    // clang-format on

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

    // clang-format off

    struct IdtPtr {
        UInt16 limit;
        UInt64 base;
    } __attribute__((packed));

    struct IdtEntry {
        UInt16 baseLow;
        UInt16 selector;
        UInt8  ist;
        UInt8  flags;
        UInt16 baseMedium;
        UInt32 baseHigh;
        UInt32 __reserved__ = 0;

        constexpr IdtEntry()
            : baseLow(0),
            selector(0),
            ist(0),
            flags(0),
            baseMedium(0),
            baseHigh(0) {}

        constexpr IdtEntry(UInt8 num, UInt64 base, UInt16 sel, UInt8 flags, UInt8 ist)
            : baseLow((UInt16)(base & 0xffff)),
            selector(sel),
            ist(ist),
            flags(flags),
            baseMedium((UInt16)(base >> 16 & 0xffff)),
            baseHigh((UInt32)(base >> 32) & 0xffffffff) {}
    } __attribute__((packed));

    // clang-format on
} // namespace Kern::Platform::X64
