#include <common/typedefs.h>
#include <utils/linked_list.h>

extern const char[16] _unknownDeviceName = "<Unknown Device>";

enum class DeviceType {
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

class Device {
public:
    Device(const char* _name, DeviceBus _bus, DeviceType _type);
    Device(const char* _name)
        : Device(m_name, DeviceBus::Unknown, DeviceType::Unknown) {}
    Device(DeviceBus bus, DeviceType type)
        : Device(&_unknownDeviceName, bus, type) {}

    inline const char*          getName() { return this->m_name; }
    inline u64                  getId() { return this->m_deviceId; }
    inline DeviceType           getType() { return this->m_type; }
    inline DeviceBus            getBus() { return this->m_deviceBus; }
    inline LinkedList<Device&>& getDependencies() {
        return this->m_dependencies;
    }
    inline bool isDependentWith(Device& device) {
        return m_dependencies.contains(device);
    }
    bool isInitialized() { return (m_flags & DeviceFlags::Initialized); }
    bool isInstalled() { return (m_flags & DeviceFlags::Installed); }
    bool isWorking() {
        return (m_flags & DeviceFlags::Initialized) &&
               !(m_flags & (DeviceFlags::Exception | DeviceFlags::DriverError |
                            DeviceFlags::DriverIncompatible));
    }

    void install();

    virtual void enable()  = 0;
    virtual void disable() = 0;

protected:
    const char* m_name;
    u64         m_deviceId;

    DeviceFlags         m_flags;
    DeviceType          m_type;
    DeviceBus           m_deviceBus;
    LinkedList<Device&> m_dependencies;
};