#include <arch-x86_64/tables.h>
#include <drivers/apic/device.h>
#include <siberix/hwtypes.h>

namespace Kern::Platform::X64 {
    struct CPUImplX64 : public Kern::Hal::CPU
    {
        uint32_t         m_apicId;
        GdtPackage       m_gdt;
        GdtPtr           m_gdtPtr;
        IdtPtr           m_idtPtr;
        TaskStateSegment m_tss __attribute__((aligned(0x10)));

        Hal::Impls::ApicDevice::ApicLocal* m_apicLocal;

        CPUImplX64(uint32_t apicId, uint32_t processorId)
          : m_apicId(apicId)
          , CPU(processorId)
        {
        }

        void disableInterrupts() override { asm volatile("cli"); }
        void enableInterrupts() override { asm volatile("sti"); }
    };
}