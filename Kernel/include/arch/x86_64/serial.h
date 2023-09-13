#include <common/logger.h>
#include <siberix/device/device.h>
#include <utils/spinlock.h>

enum SerialPort : u16
{
    COM1 = 0x3f8,
    COM2 = 0x2f8,
    COM3 = 0x3e8,
    COM4 = 0x2e8,
    COM5 = 0x5f8,
    COM6 = 0x4f8,
    COM7 = 0x5e8,
    COM8 = 0x4e8
};

enum SerialPortOffset : u16
{
    InterruptEnable = 1,
    FIFO            = 2,
    LineControl     = 3,
    ModemControl    = 4,
    LineStatus      = 5,
    ModemStatus     = 6,
    Scratch         = 7
};

inline u16
operator+(SerialPort port, SerialPortOffset offset)
{
    return static_cast<u16>(port) + static_cast<u16>(offset);
}

class SerialPortDevice : public Device
{
public:
    SerialPortDevice();
    ~SerialPortDevice();

    virtual void enable() override;
    virtual void disable() override;
};

struct SerialPortLoggerReceiver : public LoggerReceiver
{
public:
    constexpr SerialPortLoggerReceiver()
      : m_device(nullptr)
      , m_lock(Spinlock())
    {
    }
    constexpr SerialPortLoggerReceiver(SerialPortDevice* device)
      : m_device(device)
      , m_lock(Spinlock())
    {
    }
    ~SerialPortLoggerReceiver() {}

    virtual void write(char ch) override;
    virtual void write(const char* str) override;

private:
    SerialPortDevice* m_device = nullptr;
    Spinlock          m_lock;
};