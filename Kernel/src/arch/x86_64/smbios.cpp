#include <arch/x86_64/smbios.hpp>

SmbiosDevice::SmbiosDevice() {
    u64  address    = 0xf0000;
    auto doChecksum = [](u64 addr) -> bool {
        u8 checksum = 0;
        for (int i = 0; i < *((u8 *)(addr + 0x5)); i++) {
            checksum += *((u8 *)(addr + i));
        }
        return !checksum;
    }

    while (address < 0x100000) {}
}

SmbiosDevice::~SmbiosDevice() {}