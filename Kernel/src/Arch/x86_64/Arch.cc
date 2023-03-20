#include <Arch/x86_64/ports.h>
#include <Init/BootInfo.h>
#include <kern.h>

using namespace Ports;

namespace System
{
    void Halt() {
        asm("cli");
        while (true)
            asm("hlt");
    }

    void Reboot() {
        while(ReadByte8(0x64) & 0x02 != 0);
        WriteByte8(0x64, 0xFE);
        asm("hlt");
    }

    void Shutdown() {
        // Execute software shutdown progress.

        // WriteWord16(acpiFadt->pm1a_control_block, );
        // WriteWord16(acpiFadt->pm1b_control_block, );
    }
} // namespace System


