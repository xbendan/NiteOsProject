#include <common/logger.h>
#include <utils/spinlock.h>

#include <siberix/device/device.hpp>

enum class SerialPort {
    COM1 = 0x3f8,
    COM2 = 0x2f8,
    COM3 = 0x3e8,
    COM4 = 0x2e8,
    COM5 = 0x5f8,
    COM6 = 0x4f8,
    COM7 = 0x5e8,
    COM8 = 0x4e8
};

enum class SerialPortOffset {
    InterruptEnable = 1,
    FIFO            = 2,
    LineControl     = 3,
    ModemControl    = 4,
    LineStatus      = 5,
    ModemStatus     = 6,
    Scratch         = 7
};

class SerialPortLoggerReceiver : public LoggerReceiver {
public:
    SerialPortLoggerReceiver();
    ~SerialPortLoggerReceiver();

    void receive(char c) override;
    void receive(const char* str) override;

private:
    spinlock_t m_lock;
};

class SerialPortDevice : public Device {
public:
    SerialPortDevice();
    ~SerialPortDevice();
};