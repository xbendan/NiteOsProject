#include <arch/x86_64/kaddr.h>
#include <arch/x86_64/smbios.h>
#include <common/logger.h>
#include <common/string.h>
#include <siberix/core/runtimes.h>

SmbiosDevice::SmbiosDevice()
  : Device("SMBIOS", DeviceBus::Software, DeviceType::Firmware)
{
    u64  address    = IOVB(0xf0000);
    auto doChecksum = [](u64 addr) -> bool {
        u8 checksum = 0;
        for (int i = 0; i < *((u8*)(addr + 0x5)); i++) {
            checksum += *((u8*)(addr + i));
        }
        return !checksum;
    };

    while (address < IOVB(0x100000)) {
        if (memcmp((void*)address, signL2, 4) && doChecksum(address)) {
            m_version    = 2;
            m_entryPoint = (void*)address;
            break;
        }

        if (memcmp((void*)address, signL3, 5) && doChecksum(address)) {
            m_version    = 3;
            m_entryPoint = (void*)address;
            break;
        }

        address += 0x10;
    }

    if (m_version) {
        m_flags    |= DeviceInitialized;
        m_deviceId  = siberix()->getConnectivity()->registerDevice(this);
    } else {
        Logger::getLogger("smbios").error("SMBIOS structure not detected.");
    }
}

SmbiosDevice::~SmbiosDevice() {}

void
SmbiosDevice::enable()
{
}

void
SmbiosDevice::disable()
{
}