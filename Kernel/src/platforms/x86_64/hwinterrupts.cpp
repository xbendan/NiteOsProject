#include <arch-x86_64/hwinterrupts.h>

namespace Kern::Platform::X64 {
    InterruptRecord interruptRecords[256] = {
        [0]  = {"Division Error",                  INT_TYPE_FAULT,                 false},
        [1]  = { "Debug",                          (INT_TYPE_FAULT | INT_TYPE_TRAP), false},
        [2]  = { "Non-maskable Interrupt",         INT_TYPE_INTERRUPT,             false},
        [3]  = { "Breakpoint",                     INT_TYPE_TRAP,                  false},
        [4]  = { "Overflow",                       INT_TYPE_TRAP,                  false},
        [5]  = { "Bound Range Exceeded",           INT_TYPE_FAULT,                 false},
        [6]  = { "Invalid Opcode",                 INT_TYPE_FAULT,                 false},
        [7]  = { "Device Not Available",           INT_TYPE_FAULT,                 false},
        [8]  = { "Double Fault",                   INT_TYPE_ABORT,                 true },
        [9]  = { "Coprocessor Segment Overrun",    INT_TYPE_FAULT,                 false},
        [10] = { "Invalid Task State Segment",     INT_TYPE_FAULT,                 true },
        [11] = { "Segment Not Present",            INT_TYPE_FAULT,                 true },
        [12] = { "Stack-Segment Fault",            INT_TYPE_FAULT,                 true },
        [13] = { "General Protection Fault",       INT_TYPE_FAULT,                 true, GeneralProtectionFault, },
        [14] = { "Page Fault",                     INT_TYPE_FAULT,                 true, PageFault, },
        [15] = { "Reserved",                       0,                            false},
        [16] = { "x87 Floating-Point Exception",   INT_TYPE_FAULT,                 false},
        [17] = { "Alignment Check",                INT_TYPE_FAULT,                 true },
        [18] = { "Machine Check",                  INT_TYPE_ABORT,                 false},
        [19] = { "SIMD Floating-Point Exception",  INT_TYPE_FAULT,                 false},
        [20] = { "Virtualization Exception",       INT_TYPE_FAULT,                 false},
        [21] = { "Control Protection Exception",   INT_TYPE_FAULT,                 false},
        [22] = { "Reserved",                       0,                            false},
        [23] = { "Reserved",                       0,                            false},
        [24] = { "Reserved",                       0,                            false},
        [25] = { "Reserved",                       0,                            false},
        [26] = { "Reserved",                       0,                            false},
        [27] = { "Reserved",                       0,                            false},
        [28] = { "Hypervisor Injection Exception", INT_TYPE_FAULT,                 false},
        [29] = { "VMM Communication Exception",    INT_TYPE_FAULT,                 true },
        [30] = { "Security Exception",             INT_TYPE_FAULT,                 true }
    };

    void DivisionError(Registers* context) {}
}