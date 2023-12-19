#include <stdcxx/copy.h>
#include <stdcxx/linked-list.h>
#include <stdcxx/string.h>
#include <stdcxx/uuid.h>

namespace Kern::Device {
    enum DeviceType
    {
        Biometric,
        Bluetooth,
        DiskDrive,
        DiskController,
        DisplayAdapter,
        DisplayMonitor,
        Firmware,
        HumanInterfaceDevice,
        Keyboard,
        Multimedia,
        NetworkAdapter,
        Portable,
        PointerDevice,
        Printer,
        PowerSupply,
        Security,
        SoftwareDevice,
        Processor,
        SystemDevices,
        USBController,
        Sensor,

        Unknown
    };

    enum DeviceBus
    {
        Software = 0,
        PCI      = 1,
        USB      = 2,
        PS2      = 3,
        ISA      = 4,

        Unknown = 5
    };

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
        Device(Std::String<Utf16> name, DeviceBus bus, DeviceType type);
        Device(Std::String<Utf16> name);
        Device(DeviceBus bus, DeviceType type);

        inline Std::String<Utf16> getName() { return this->m_name; }
        inline UInt64             getId() { return this->m_deviceId; }
        inline DeviceBus          getBus() { return this->m_bus; }
        inline DeviceType         getType() { return this->m_type; }
        inline UInt64             getFlags() { return this->m_flags; }

        Device& operator=(Device&& other)
        {
            if (this != &other) {
                m_name         = Std::Move(other.m_name);
                m_deviceId     = other.m_deviceId;
                m_flags        = other.m_flags;
                m_bus          = other.m_bus;
                m_type         = other.m_type;
                m_dependencies = Std::Move(other.m_dependencies);
            }
            return *this;
        }

    private:
        Std::String<Utf16> m_name;
        Std::UUID          m_uuid;
        UInt64             m_deviceId;
        UInt64             m_flags;

        DeviceBus                m_bus;
        DeviceType               m_type;
        Std::LinkedList<Device&> m_dependencies;
    };
}