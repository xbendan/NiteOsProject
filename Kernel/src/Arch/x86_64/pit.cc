#include <Arch/x86_64/PIT.h>
#include <Arch/x86_64/Ports.h>

using namespace Ports;

namespace PIT
{
    void Initialize(uint32_t frequency)
    {
        uint16_t __divisor__ = PIT_FREQUENCY / frequency;

        WriteByte8(0x43, 0x36);
        WriteByte8(0x40, __divisor__ & 0xFF);
        WriteByte8(0x40, (__divisor__ >> 8) & 0xFF);
    }
} // namespace PIT