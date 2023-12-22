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
        uint16_t limit;
        uint64_t base;
    } __attribute__((packed));

    struct GdtEntry {
        uint16_t limitLow;
        uint16_t baseLow;
        uint8_t  baseMedium;
        uint8_t  access;
        uint8_t  granularity;
        uint8_t  baseHigh;
    } __attribute__((packed));

    struct GdtExtraEntry {
        uint32_t baseHighest;
        uint32_t __reserved__;
    } __attribute__((packed));

    struct GdtPackage;

    class TaskStateSegment {
    public:
        uint32_t __reserved__0 __attribute__((aligned(0x10)));
        uint64_t rsp[3];
        uint64_t __reserved__1;
        uint64_t ist[7];
        uint32_t __reserved__2;
        uint32_t __reserved__3;
        uint16_t __reserved__4;
        uint16_t iopbOffset;

        void init(GdtPackage *package);
    } __attribute__((packed));

    struct GdtTssEntry {
        uint16_t len;
        uint16_t baseLow;
        uint8_t  baseMedium;
        uint8_t  flags_a;
        uint8_t  flags_b;
        uint8_t  baseHigh;
        uint32_t baseUpper;
        uint32_t __reserved__;

        GdtTssEntry() {}
        GdtTssEntry(TaskStateSegment &tss)
            : len(sizeof(TaskStateSegment)),
            baseLow(((uint64_t)&tss) & 0xffff),
            baseMedium(((uint64_t)&tss) >> 16 & 0xff),
            flags_a(0b10001001),
            flags_b(0),
            baseHigh(((uint64_t)&tss) >> 24 & 0xff),
            baseUpper(((uint64_t)&tss) >> 32 & 0xffffffff),
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
        uint16_t limit;
        uint64_t base;
    } __attribute__((packed));

    struct IdtEntry {
        uint16_t baseLow;
        uint16_t selector;
        uint8_t  ist;
        uint8_t  flags;
        uint16_t baseMedium;
        uint32_t baseHigh;
        uint32_t __reserved__ = 0;

        constexpr IdtEntry()
            : baseLow(0),
            selector(0),
            ist(0),
            flags(0),
            baseMedium(0),
            baseHigh(0) {}

        constexpr IdtEntry(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags, uint8_t ist)
            : baseLow((uint16_t)(base & 0xffff)),
            selector(sel),
            ist(ist),
            flags(flags),
            baseMedium((uint16_t)(base >> 16 & 0xffff)),
            baseHigh((uint32_t)(base >> 32) & 0xffffffff) {}
    } __attribute__((packed));

    // clang-format on
} // namespace Kern::Platform::X64
