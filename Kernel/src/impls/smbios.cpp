#include <drivers/smbios/device.h>
#include <siberix/mem/mem.h>
#include <siberix/mem/range.h>

namespace Kern::Hal::Impls {
    using namespace Specs;

    const char* __smbios_SignatureL2 = "_SM_";
    const char* __smbios_SignatureL3 = "_SM3_";

    SmbiosDevice::SmbiosDevice()
      : Device("System Management BIOS Device", DeviceType::Firmware)
    {
        uint64_t          address = Mem::copyAsIoAddress(0xF0000);
        Mem::AddressRange range(address, 0x10000);

        if ((address = range.find(__smbios_SignatureL2, 4, 0x10)) != 0) {
            m_majorVer   = 2;
            m_smbiosInfo = (void*)address;
        } else if ((address = range.find(__smbios_SignatureL3, 5, 0x10)) != 0) {
            m_majorVer   = 3;
            m_smbiosInfo = (void*)address;
        }
    }
}