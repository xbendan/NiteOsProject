#include <arch/x86_64/arch.h>

#include <arch/x86_64/iopt.h>
#include <arch/x86_64/serial.h>

SerialPortDevice::SerialPortDevice() {
    outByte8(SerialPort::COM1 + SerialPortOffset::InterruptEnable, 0x00);
    outByte8(SerialPort::COM1 + SerialPortOffset::LineControl, 0x80);
    outByte8(SerialPort::COM1, 0x03);
    outByte8(SerialPort::COM1, 0x00);
    outByte8(SerialPort::COM1 + SerialPortOffset::LineControl, 0x03);
    outByte8(SerialPort::COM1 + SerialPortOffset::FIFO, 0xc7);
    outByte8(SerialPort::COM1 + SerialPortOffset::ModemControl, 0x0b);

    m_flags |= (DeviceFlags::Initialized);

    Logger::getLoggerReceivers().add(new SerialPortLoggerReceiver());
}

SerialPortDevice::~SerialPortDevice() {}

SerialPortLoggerReceiver::SerialPortLoggerReceiver() {}

SerialPortLoggerReceiver::~SerialPortLoggerReceiver() {}

SerialPortLoggerReceiver::receive(char c) {
    while (!(inByte8(SerialPort::COM1 + SerialPortOffset::LineStatus) & 0x20))
        ;
    outByte8(SerialPort::COM1, c);
}

SerialPortLoggerReceiver::receive(const char* str) {
    int len = strlen(str);
    if (checkInterrupts()) {
        acquireLockIntDisable(&m_lock);
        while (len--) {
            while (!(inByte8(SerialPort::COM1 + SerialPortOffset::LineStatus) &
                     0x20))
                ;
            outByte8(SerialPort::COM1, *str++);
        }

        releaseLock(&m_lock);
        asm("sti");
    } else
        while (len--) {
            while (!(inByte8(SerialPort::COM1 + SerialPortOffset::LineStatus) &
                     0x20))
                ;
            outByte8(SerialPort::COM1, *str++);
        }
}