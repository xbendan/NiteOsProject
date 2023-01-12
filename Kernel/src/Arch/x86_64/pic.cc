#include <Arch/x86_64/ports.h>
#include <Arch/x86_64/pic.h>

using namespace Ports;

namespace PIC
{
    void Initialize()
    {
        WriteByte8(0x20, 0x11);
        WriteByte8(0xA0, 0x11);

        WriteByte8(0x21, 0x20);
        WriteByte8(0xA1, 0x28);

        WriteByte8(0x21, 0x04);
        WriteByte8(0xA1, 0x02);

        WriteByte8(0x21, 0x01);
        WriteByte8(0xA1, 0x01);

        WriteByte8(0x21, 0x0);
        WriteByte8(0xA1, 0x0);
    }

    void Disable()
    {
        WriteByte8(0xA1, 0xFF);
        WriteByte8(0x21, 0xFF);
    }
} // namespace PIC

void RestoreInterrupts(int i)
{
    if (i >= 40)
    {
        WriteByte8(0xA0, 0x20);
    }

    WriteByte8(0x20, 0x20);
}