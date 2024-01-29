#pragma once

#include <stdcxx/linked-list.h>
#include <stdcxx/move.h>
#include <stdcxx/string.h>
#include <stdcxx/uuid.h>

#define DEVICE_FLAG_LOADED 0x01

namespace Kern {
    enum DeviceType
    {
        Biometric,
        Bluetooth,
        DiskDrive,
        DiskController,
        DisplayAdapter,
        DisplayOutput,
        Firmware,
        HumanInterfaceDevice,
        Keyboard,
        Multimedia,
        NetworkAdapter,
        Portable,
        PointerDevice,
        Printer,
        PowerSupply,
        Processor,
        Security,
        SoftwareDevice,
        SystemDevices,
        Sensor,
        TimerOrClock,
        USBController,

        Unknown
    };

    // enum DeviceBus
    // {
    //     Software = 0,
    //     PCI      = 1,
    //     USB      = 2,
    //     PS2      = 3,
    //     ISA      = 4,

    //     Unknown = 5
    // };

    enum DeviceFlag
    {
        Initialized        = 0x01,
        Installed          = 0x02,
        ExceptionOccurred  = 0x04,
        DriverError        = 0x08,
        DriverIncompatible = 0x10
    };

    class Device
    {
    public:
        Device(Std::String<Utf8> name, DeviceType type);
        Device(Std::String<Utf8> name);
        // Device(DeviceType type);

        inline Std::String<Utf8> getName() { return this->m_name; }
        inline uint64_t          getId() { return this->m_deviceId; }
        // inline DeviceBus         getBus() { return this->m_bus; }
        inline DeviceType        getType() { return this->m_type; }
        inline uint64_t          getFlags() { return this->m_flags; }

        Device& operator=(Device&& other)
        {
            if (this != &other) {
                m_name         = Std::move(other.m_name);
                m_uuid         = Std::move(other.m_uuid);
                m_deviceId     = other.m_deviceId;
                m_flags        = other.m_flags;
                m_type         = other.m_type;
                m_dependencies = Std::move(other.m_dependencies);
            }
            return *this;
        }

    protected:
        Std::String<Utf8> m_name;
        Std::UUID         m_uuid;
        uint64_t          m_deviceId;
        uint64_t          m_flags;

        DeviceType               m_type;
        Std::LinkedList<Device*> m_dependencies;
    };
}