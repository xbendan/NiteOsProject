#include <Arch/x86_64/Ports.h>
#include <Kernel>

using namespace Ports;

void MakeTripleFault()
{
    while(ReadByte8(0x64) & 0x02 != 0);
    WriteByte8(0x64, 0xFE);
    asm("hlt");
}