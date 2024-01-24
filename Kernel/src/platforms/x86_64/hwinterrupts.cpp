#include <arch-x86_64/hwinterrupts.h>
#include <siberix/hwtypes.h>

namespace Kern::Hal {
    // clang-format off
    void CPU::enableInterrupts() { asm volatile("sti"); }
    void CPU::disableInterrupts() { asm volatile("cli"); }
    // clang-format on
}

namespace Kern::Platform::X64 {
    IdtEntry idtEntry[256] = {};

    InterruptRecord interruptRecords[256] = {
        [0]  = {"Division Error",                  INT_TYPE_FAULT,                  false},
        [1]  = { "Debug",                          (INT_TYPE_FAULT | INT_TYPE_TRAP), false},
        [2]  = { "Non-maskable Interrupt",         INT_TYPE_INTERRUPT,              false},
        [3]  = { "Breakpoint",                     INT_TYPE_TRAP,                   false},
        [4]  = { "Overflow",                       INT_TYPE_TRAP,                   false},
        [5]  = { "Bound Range Exceeded",           INT_TYPE_FAULT,                  false},
        [6]  = { "Invalid Opcode",                 INT_TYPE_FAULT,                  false},
        [7]  = { "Device Not Available",           INT_TYPE_FAULT,                  false},
        [8]  = { "Double Fault",                   INT_TYPE_ABORT,                  true },
        [9]  = { "Coprocessor Segment Overrun",    INT_TYPE_FAULT,                  false},
        [10] = { "Invalid Task State Segment",     INT_TYPE_FAULT,                  true },
        [11] = { "Segment Not Present",            INT_TYPE_FAULT,                  true },
        [12] = { "Stack-Segment Fault",            INT_TYPE_FAULT,                  true },
        [13] = { "General Protection Fault",       INT_TYPE_FAULT,                  true, GeneralProtectionFault, },
        [14] = { "Page Fault",                     INT_TYPE_FAULT,                  true, PageFault, },
        [15] = { "Reserved",                       0,                               false},
        [16] = { "x87 Floating-Point Exception",   INT_TYPE_FAULT,                  false},
        [17] = { "Alignment Check",                INT_TYPE_FAULT,                  true },
        [18] = { "Machine Check",                  INT_TYPE_ABORT,                  false},
        [19] = { "SIMD Floating-Point Exception",  INT_TYPE_FAULT,                  false},
        [20] = { "Virtualization Exception",       INT_TYPE_FAULT,                  false},
        [21] = { "Control Protection Exception",   INT_TYPE_FAULT,                  false},
        [22] = { "Reserved",                       0,                               false},
        [23] = { "Reserved",                       0,                               false},
        [24] = { "Reserved",                       0,                               false},
        [25] = { "Reserved",                       0,                               false},
        [26] = { "Reserved",                       0,                               false},
        [27] = { "Reserved",                       0,                               false},
        [28] = { "Hypervisor Injection Exception", INT_TYPE_FAULT,                  false},
        [29] = { "VMM Communication Exception",    INT_TYPE_FAULT,                  true },
        [30] = { "Security Exception",             INT_TYPE_FAULT,                  true }
    };

    void UnhandledException(Registers* context) {}

    void DivisionError(Registers* context) {}

    void Debug(Registers* context) {}

    void NonMaskableInterrupt(Registers* context) {}

    void Breakpoint(Registers* context) {}

    void Overflow(Registers* context) {}

    void BoundRangeExceeded(Registers* context) {}

    void InvalidOpcode(Registers* context) {}

    void DeviceNotAvailable(Registers* context) {}

    void DoubleFault(Registers* context) {}

    void CoprocessorSegmentOverrun(Registers* context) {}

    void InvalidTaskStateSegment(Registers* context) {}

    void SegmentNotPresent(Registers* context) {}

    void StackSegmentFault(Registers* context) {}

    void GeneralProtectionFault(Registers* context) {}

    void PageFault(Registers* context) {}

    void x87FloatingPointException(Registers* context) {}

    void AlignmentCheck(Registers* context) {}

    void MachineCheck(Registers* context) {}

    void SIMDFloatingPointException(Registers* context) {}

    void VirtualizationException(Registers* context) {}

    void ControlProtectionException(Registers* context) {}

    void HypervisorInjectionException(Registers* context) {}

    void VMMCommunicationException(Registers* context) {}

    void SecurityException(Registers* context) {}
}