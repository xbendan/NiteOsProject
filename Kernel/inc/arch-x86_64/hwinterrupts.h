#include <arch-x86_64/hwregs.h>
#include <arch-x86_64/tables.h>
#include <stdcxx/func.h>
#include <stdcxx/string.h>
#include <stdcxx/types.h>

#define INT_TYPE_FAULT 0x1
#define INT_TYPE_TRAP 0x2
#define INT_TYPE_INTERRUPT 0x4
#define INT_TYPE_ABORT 0x8

extern UInt64 g_interruptTables[];

namespace Kern::Platform::X64 {
    struct InterruptRecord
    {
        const char* m_name;
        UInt8       m_type;
        Boolean     m_hasErrCode;

        Void (*m_handler)(Registers*);
    };

    Void UnhandledException(Registers* context);

    Void DivisionError(Registers* context);
    Void Debug(Registers* context);
    Void NonMaskableInterrupt(Registers* context);
    Void Breakpoint(Registers* context);
    Void Overflow(Registers* context);
    Void BoundRangeExceeded(Registers* context);
    Void InvalidOpcode(Registers* context);
    Void DeviceNotAvailable(Registers* context);
    Void DoubleFault(Registers* context);
    Void CoprocessorSegmentOverrun(Registers* context);
    Void InvalidTaskStateSegment(Registers* context);
    Void SegmentNotPresent(Registers* context);
    Void StackSegmentFault(Registers* context);
    Void GeneralProtectionFault(Registers* context);
    Void PageFault(Registers* context);
    Void AlignmentCheck(Registers* context);
    Void MachineCheck(Registers* context);
    Void SIMDException(Registers* context);
    Void VirtualizationException(Registers* context);
    Void ControlProtectionException(Registers* context);
    Void HypervisorInjectionException(Registers* context);
    Void VMMCommunicationException(Registers* context);
    Void SecurityException(Registers* context);
}