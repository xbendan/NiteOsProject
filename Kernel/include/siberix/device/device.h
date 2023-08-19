#pragma once

#include <common/typedefs.h>
#include <utils/linked_list.h>

const char* _unknownDeviceName = "<Unknown Device>";

enum class DeviceType : u8 {
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

enum class DeviceBus {
    Software = 0,
    PCI      = 1,
    USB      = 2,
    PS2      = 3,

    Unknown = 4
};

enum class DeviceFlags : u64 {
    Initialized        = 0x01,
    Installed          = 0x02,
    Exception          = 0x04,
    DriverInstalled    = 0x08,
    DriverIncompatible = 0x10,
    DriverError        = 0x20
};

bool operator==(DeviceFlags a, DeviceFlags b) { return static_cast<u64>(a) == static_cast<u64>(b); }

u64 operator|(DeviceFlags a, DeviceFlags b) { return static_cast<u64>(a) | static_cast<u64>(b); }

u64 operator|(u64 a, DeviceFlags b) { return a | static_cast<u64>(b); }

bool operator&(DeviceFlags a, DeviceFlags b) { return static_cast<u64>(a) & static_cast<u64>(b); }

bool operator&(DeviceFlags a, u64 b) { return static_cast<u64>(a) & b; }

bool operator&(u64 a, DeviceFlags b) { return a & static_cast<u64>(b); }

u64 operator|=(u64 a, DeviceFlags b) { return a | static_cast<u64>(b); }

class Device {
public:
    Device(const char* _name, DeviceBus _bus, DeviceType _type);
    Device(const char* _name);
    Device(DeviceBus bus, DeviceType type);
    ~Device();

    inline const char*          getName() { return this->m_name; }
    inline u64                  getId() { return this->m_deviceId; }
    inline DeviceType           getType() { return this->m_type; }
    inline DeviceBus            getBus() { return this->m_bus; }
    inline LinkedList<Device&>& getDependencies() { return this->m_dependencies; }
    inline bool isDependentWith(Device& device) { return m_dependencies.contains(device); }
    bool        isInitialized() { return (m_flags & DeviceFlags::Initialized); }
    bool        isInstalled() { return (m_flags & DeviceFlags::Installed); }
    bool        isWorking() {
        return (m_flags & DeviceFlags::Initialized) &&
               !(m_flags & (DeviceFlags::Exception | DeviceFlags::DriverError |
                            DeviceFlags::DriverIncompatible));
    }

    bool operator==(Device& device) { return this->m_deviceId == device.m_deviceId; }

    void initialize();

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