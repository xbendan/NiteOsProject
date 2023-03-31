#include <Arch/x86_64/Ports.h>
#include <Init/BootInfo.h>

using namespace Ports;

namespace System
{
    void Reboot() {
        while(ReadByte8(0x64));
        WriteByte8(0x64, 0xFE);
        asm("hlt");
    }

    void Shutdown() {
        // Execute software shutdown progress.

        // WriteWord16(acpiFadt->pm1a_control_block, );
        // WriteWord16(acpiFadt->pm1b_control_block, );
    }
} // namespace System

void Halt()
{
    asm("cli");
    while (true)
        asm("hlt");
}