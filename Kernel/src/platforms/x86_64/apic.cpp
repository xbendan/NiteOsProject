#include <arch-x86_64/apic.h>

namespace Kern::Platform::X64::Apic {
    ApicDevice::ApicDevice()
      : m_ioBasePhys(0xFEC00000)
    {
    }
}