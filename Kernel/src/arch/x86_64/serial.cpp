#include <arch/x86_64/arch.h>
#include <arch/x86_64/iopt.h>
#include <arch/x86_64/serial.h>

static SerialPortLoggerReceiver loggerReceiver;

SerialPortDevice::SerialPortDevice()
    : Device("Serial Port", DeviceBus::ISA, DeviceType::SoftwareDevice) {
    outByte8(COM1 + SerialPortOffset::InterruptEnable, 0x00);
    outByte8(COM1 + SerialPortOffset::LineControl, 0x80);
    outByte8(COM1, 0x03);
    outByte8(COM1, 0x00);
    outByte8(COM1 + SerialPortOffset::LineControl, 0x03);
    outByte8(COM1 + SerialPortOffset::FIFO, 0xc7);
    outByte8(COM1 + SerialPortOffset::ModemControl, 0x0b);

    m_flags        |= DeviceInitialized;
    loggerReceiver  = SerialPortLoggerReceiver();

    Logger::getLoggerReceivers().add(loggerReceiver);
}

SerialPortDevice::~SerialPortDevice() {}

void SerialPortDevice::enable() {}

void SerialPortDevice::disable() {}

SerialPortLoggerReceiver::SerialPortLoggerReceiver() {}

SerialPortLoggerReceiver::~SerialPortLoggerReceiver() {}

void SerialPortLoggerReceiver::receive(char c) {
    while (!(inByte8(SerialPort::COM1 + SerialPortOffset::LineStatus) & 0x20))
        ;
    outByte8(SerialPort::COM1, c);
}

void SerialPortLoggerReceiver::receive(const char* str) {
    int len = strlen(str);
    if (checkInterrupts()) {
        acquireLockIntDisable(&m_lock);
        while (len--) {
            while (!(inByte8(SerialPort::COM1 + SerialPortOffset::LineStatus) & 0x20))
                ;
            outByte8(SerialPort::COM1, *str++);
        }

        releaseLock(&m_lock);
        asm("sti");
    } else
        while (len--) {
            while (!(inByte8(SerialPort::COM1 + SerialPortOffset::LineStatus) & 0x20))
                ;
            outByte8(SerialPort::COM1, *str++);
        }
}