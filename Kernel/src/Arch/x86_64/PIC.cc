#include <Arch/x86_64/Ports.h>
#include <Arch/x86_64/pic.h>

using namespace Ports;

namespace PIC
{
    void Initialize()
    {
        WriteByte8(0x20, 0x11); IoWait();
        WriteByte8(0xA0, 0x11); IoWait();

        WriteByte8(0x21, 0x20); IoWait();
        WriteByte8(0xA1, 0x28); IoWait();

        WriteByte8(0x21, 0x04); IoWait();
        WriteByte8(0xA1, 0x02); IoWait();

        WriteByte8(0x21, 0x01); IoWait();
        WriteByte8(0xA1, 0x01); IoWait();

        WriteByte8(0x21, 0xff); IoWait();
        WriteByte8(0xA1, 0xff); IoWait();
    }

    void Disable()
    {
        // WriteByte8(0x20, 0x11); IoWait();
        // WriteByte8(0xA0, 0x11); IoWait();
        // WriteByte8(0x21, 0x20); IoWait();// Remap IRQs on both PICs to 0xF0-0xF8
        // WriteByte8(0xA1, 0x28); IoWait();
        // WriteByte8(0x21, 0x04); IoWait();
        // WriteByte8(0xA1, 0x02); IoWait();
        // WriteByte8(0x21, 0x01); IoWait();
        // WriteByte8(0xA1, 0x01); IoWait();

        WriteByte8(0x21, 0xFF); // Mask all interrupts
        WriteByte8(0xA1, 0xFF);
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