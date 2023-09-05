#include <common/logger.h>
#include <siberix/device/device.h>
#include <utils/spinlock.h>

enum SerialPort {
    COM1 = 0x3f8,
    COM2 = 0x2f8,
    COM3 = 0x3e8,
    COM4 = 0x2e8,
    COM5 = 0x5f8,
    COM6 = 0x4f8,
    COM7 = 0x5e8,
    COM8 = 0x4e8
};

enum SerialPortOffset {
    InterruptEnable = 1,
    FIFO            = 2,
    LineControl     = 3,
    ModemControl    = 4,
    LineStatus      = 5,
    ModemStatus     = 6,
    Scratch         = 7
};

class SerialPortDevice : public Device {
public:
    SerialPortDevice();
    ~SerialPortDevice();

    void enable() override;
    void disable() override;
};

class SerialPortLoggerReceiver : public LoggerReceiver {
public:
    SerialPortLoggerReceiver() = default;
    SerialPortLoggerReceiver(SerialPortDevice* device);
    ~SerialPortLoggerReceiver();

    void receive(char c) override;
    void receive(const char* str) override;

private:
    SerialPortDevice* m_device;
    spinlock_t        m_lock;
};