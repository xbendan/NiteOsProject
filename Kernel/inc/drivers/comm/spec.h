#include <stdcxx/types.h>

namespace Kern::Hal::Impls {
    enum SerialPort : uint16_t
    {
        COM1 = 0x3F8,
        COM2 = 0x2F8,
        COM3 = 0x3E8,
        COM4 = 0x2E8,
        COM5 = 0x5F8,
        COM6 = 0x4F8,
        COM7 = 0x5E8,
        COM8 = 0x4E8
    };

    enum SerialPortOffset : uint16_t
    {
        InterruptEnable = 1,
        FIFO            = 2,
        LineControl     = 3,
        ModemControl    = 4,
        LineStatus      = 5,
        ModemStatus     = 6,
        Scratch         = 7
    };
}