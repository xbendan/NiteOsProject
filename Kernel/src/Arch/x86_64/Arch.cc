#include <Arch/x86_64/ports.h>
#include <init/bootinfo.h>
#include <kern.h>

using namespace Ports;

void MakeTripleFault()
{
    while(ReadByte8(0x64) & 0x02 != 0);
    WriteByte8(0x64, 0xFE);
    asm("hlt");
}

namespace System
{
    void Halt() {
        asm("cli");
        while (true)
            asm("hlt");
    }
} // namespace System


