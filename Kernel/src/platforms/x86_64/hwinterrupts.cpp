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
        [0]  = {"Division Error",                  INT_TYPE_FAULT,                  false,  DivisionError },
        [1]  = { "Debug",                          (INT_TYPE_FAULT | INT_TYPE_TRAP), false, Debug },
        [2]  = { "Non-maskable Interrupt",         INT_TYPE_INTERRUPT,              false,  NonMaskableInterrupt },
        [3]  = { "Breakpoint",                     INT_TYPE_TRAP,                   false,  Breakpoint },
        [4]  = { "Overflow",                       INT_TYPE_TRAP,                   false,  Overflow },
        [5]  = { "Bound Range Exceeded",           INT_TYPE_FAULT,                  false,  BoundRangeExceeded },
        [6]  = { "Invalid Opcode",                 INT_TYPE_FAULT,                  false,  InvalidOpcode },
        [7]  = { "Device Not Available",           INT_TYPE_FAULT,                  false,  DeviceNotAvailable },
        [8]  = { "Double Fault",                   INT_TYPE_ABORT,                  true,   DoubleFault },
        [9]  = { "Coprocessor Segment Overrun",    INT_TYPE_FAULT,                  false,  CoprocessorSegmentOverrun },
        [10] = { "Invalid Task State Segment",     INT_TYPE_FAULT,                  true,   InvalidTaskStateSegment },
        [11] = { "Segment Not Present",            INT_TYPE_FAULT,                  true,   SegmentNotPresent },
        [12] = { "Stack-Segment Fault",            INT_TYPE_FAULT,                  true,   StackSegmentFault },
        [13] = { "General Protection Fault",       INT_TYPE_FAULT,                  true,   GeneralProtectionFault, },
        [14] = { "Page Fault",                     INT_TYPE_FAULT,                  true,   PageFault, },
        [15] = { "Reserved",                       0,                               false,  UnhandledException },
        [16] = { "x87 Floating-Point Exception",   INT_TYPE_FAULT,                  false,  x87FloatingPointException },
        [17] = { "Alignment Check",                INT_TYPE_FAULT,                  true,   AlignmentCheck },
        [18] = { "Machine Check",                  INT_TYPE_ABORT,                  false,  MachineCheck },
        [19] = { "SIMD Floating-Point Exception",  INT_TYPE_FAULT,                  false,  SIMDFloatingPointException },
        [20] = { "Virtualization Exception",       INT_TYPE_FAULT,                  false,  VirtualizationException },
        [21] = { "Control Protection Exception",   INT_TYPE_FAULT,                  false,  ControlProtectionException },
        [22] = { "Reserved",                       0,                               false,  UnhandledException },
        [23] = { "Reserved",                       0,                               false,  UnhandledException },
        [24] = { "Reserved",                       0,                               false,  UnhandledException },
        [25] = { "Reserved",                       0,                               false,  UnhandledException },
        [26] = { "Reserved",                       0,                               false,  UnhandledException },
        [27] = { "Reserved",                       0,                               false,  UnhandledException },
        [28] = { "Hypervisor Injection Exception", INT_TYPE_FAULT,                  false,  HypervisorInjectionException },
        [29] = { "VMM Communication Exception",    INT_TYPE_FAULT,                  true,   VMMCommunicationException },
        [30] = { "Security Exception",             INT_TYPE_FAULT,                  true,   SecurityException }
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