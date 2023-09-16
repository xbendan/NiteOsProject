#pragma once

#include <common/typedefs.h>
#include <utils/linked_list.h>

extern const char* _unknownDeviceName;

enum class DeviceType : u8
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

enum class DeviceBus
{
    Software = 0,
    PCI      = 1,
    USB      = 2,
    PS2      = 3,
    ISA      = 4,

    Unknown = 5
};

enum DeviceFlags : u64
{
    DeviceInitialized        = 0x01,
    DeviceInstalled          = 0x02,
    DeviceExceptionOccurred  = 0x04,
    DeviceDriverError        = 0x08,
    DeviceDriverIncompatible = 0x10
};

class Device
{
public:
    Device(const char* _name, DeviceBus _bus, DeviceType _type);
    Device(const char* _name);
    Device(DeviceBus bus, DeviceType type);
    ~Device();

    inline const char*          getName() { return this->m_name; }
    inline u64                  getId() { return this->m_deviceId; }
    inline DeviceType           getType() { return this->m_type; }
    inline DeviceBus            getBus() { return this->m_bus; }
    inline LinkedList<Device&>& getDependencies()
    {
        return this->m_dependencies;
    }
    inline bool isDependentWith(Device& device)
    {
        return m_dependencies.contains(device);
    }
    bool isInitialized() { return (m_flags & DeviceInitialized); }
    bool isInstalled() { return (m_flags & DeviceInstalled); }
    bool isWorking()
    {
        return (m_flags & DeviceInitialized) &&
               !(m_flags & (DeviceExceptionOccurred | DeviceDriverError |
                            DeviceDriverIncompatible));
    }

    bool operator==(Device& device)
    {
        return this->m_deviceId == device.m_deviceId;
    }

    virtual void enable()  = 0;
    virtual void disable() = 0;

protected:
    const char* m_name;
    u64         m_deviceId;

    u64                 m_flags;
    DeviceBus           m_bus;
    DeviceType          m_type;
    LinkedList<Device&> m_dependencies;
};