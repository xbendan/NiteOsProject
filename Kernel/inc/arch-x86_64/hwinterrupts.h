#include <arch-x86_64/hwregs.h>
#include <arch-x86_64/tables.h>
#include <stdcxx/func.h>
#include <stdcxx/string.h>
#include <stdcxx/types.h>

#define INT_TYPE_FAULT 0x1
#define INT_TYPE_TRAP 0x2
#define INT_TYPE_INTERRUPT 0x4
#define INT_TYPE_ABORT 0x8

extern uint64_t g_interruptTables[];

namespace Kern::Platform::X64 {
    struct InterruptRecord
    {
        const char* m_name;
        uint8_t       m_type;
        bool     m_hasErrCode;

        void (*m_handler)(Registers*);
    };

    void UnhandledException(Registers* context);

    void DivisionError(Registers* context);
    void Debug(Registers* context);
    void NonMaskableInterrupt(Registers* context);
    void Breakpoint(Registers* context);
    void Overflow(Registers* context);
    void BoundRangeExceeded(Registers* context);
    void InvalidOpcode(Registers* context);
    void DeviceNotAvailable(Registers* context);
    void DoubleFault(Registers* context);
    void CoprocessorSegmentOverrun(Registers* context);
    void InvalidTaskStateSegment(Registers* context);
    void SegmentNotPresent(Registers* context);
    void StackSegmentFault(Registers* context);
    void GeneralProtectionFault(Registers* context);
    void PageFault(Registers* context);
    void AlignmentCheck(Registers* context);
    void MachineCheck(Registers* context);
    void SIMDException(Registers* context);
    void VirtualizationException(Registers* context);
    void ControlProtectionException(Registers* context);
    void HypervisorInjectionException(Registers* context);
    void VMMCommunicationException(Registers* context);
    void SecurityException(Registers* context);
}