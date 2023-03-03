#include <Arch/x86_64/smbios.h>
#include <Arch/x86_64/mmu.h>
#include <address.h>
#include <kern.h>

namespace SMBios
{
    const char* __smbios_SignatureL2 = "_SM_";
    const char* __smbios_SignatureL3 = "_SM3_";

    int g_SmbiosVersion = -1;
    void *SmbiosInfo;

    void Initialize()
    {
        uintptr_t address = 0xF0000 + IO_VIRTUAL_BASE;

        auto checksum = [](uint64_t addr) -> bool {
            uint8_t _checksum = 0;
            for(int i = 0; i < *((uint8_t *)(addr + 0x5)); i++)
                _checksum += *((uint8_t *)(addr + i));
            return !_checksum;
        };

        while (address < 0x100000 + IO_VIRTUAL_BASE)
        {
            if(memcmp((void *) address, __smbios_SignatureL2, 4)
                && checksum(address))
            {
                g_SmbiosVersion = 2;
                SmbiosInfo = (void *) address;
                break;
            }

            if(memcmp((void *) address, __smbios_SignatureL3, 5)
                && checksum(address))
            {
                g_SmbiosVersion = 3;
                SmbiosInfo = (void *) address;
                break;
            }

            address += 0x10;
        }

        if(g_SmbiosVersion == -1)
            CallPanic("[SMBIOS] No SMBIOS found!");
    }
}