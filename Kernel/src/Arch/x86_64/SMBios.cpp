#include <Arch/x86_64/SMBios.h>
#include <Arch/x86_64/Paging.h>
#include <Address>
#include <Kernel>

namespace SMBios
{
    const char* __smbios_SignatureL2 = "_SM_";
    const char* __smbios_SignatureL3 = "_SM3_";

    int g_SmbiosVersion = -1;
    void *SmbiosInfo;

    void Checksum(uint64_t addr)
    {
        int length = *((uint8_t *)(address + 0x5));
        uint8_t checksum = 0;
        for(int i = 0; i < length; i++)
            checksum += *((uint8_t *)(address + i));
        return !checksum;
    }

    void Initialize()
    {
        uintptr_t address = 0xF0000 + KERNEL_IO_VIRTUAL_BASE;
        while (address < 0x100000 + KERNEL_IO_VIRTUAL_BASE)
        {
            if(memcmp((void *) address, __smbios_SignatureL2, 4)
                && Checksum(address))
            {
                g_SmbiosVersion = 2;
                SmbiosInfo = (void *) address;
                break;
            }

            if(memcmp((void *) address, __smbios_SignatureL3, 5)
                && Checksum(address))
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