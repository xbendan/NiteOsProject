#include <arch-x86_64/tables.h>
#include <siberix/hwtypes.h>

namespace Kern::Platform::X64 {
    struct CPUImplX64 : public Kern::Hal::CPU
    {
        uint32_t         m_apicId;
        GdtPackage       m_gdt;
        GdtPtr           m_gdtPtr;
        IdtPtr           m_idtPtr;
        TaskStateSegment m_tss __attribute__((aligned(0x10)));

        CPUImplX64(uint32_t apicId, uint32_t processorId)
          : m_apicId(apicId)
          , CPU(processorId)
        {
        }
    };
}