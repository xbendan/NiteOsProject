#include <arch-x86_64/tables.h>
#include <siberix/hwtypes.h>

namespace Kern::Platform::X64 {
    struct CpuArch : public Kern::Hal::Cpu
    {
        constexpr CpuArch(UInt32 processorId) {}

        UInt32           m_apicId;
        GdtPackage*      m_gdt;
        GdtPtr           m_gdtPtr;
        IdtPtr           m_idtPtr;
        TaskStateSegment m_tss __attribute__((aligned(0x10)));
    };
}