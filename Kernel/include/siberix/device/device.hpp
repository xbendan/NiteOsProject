#include <common/typedefs.h>
#include <utils/linked_list.h>

extern const char[16] _unknownDeviceName = "<Unknown Device>";

enum DeviceType {
    DeviceTypeBiometric,
    DeviceTypeBluetooth,
    DeviceTypeDiskDrive,
    DeviceTypeDiskController,
    DeviceTypeDisplayAdapter,
    DeviceTypeDisplayMonitor,
    DeviceTypeFirmware,
    DeviceTypeHumanInterfaceDevice,
    DeviceTypeKeyboard,
    DeviceTypeMultimedia,
    DeviceTypeNetworkAdapter,
    DeviceTypePortable,
    DeviceTypePointerDevice,
    DeviceTypePrinter,
    DeviceTypePowerSupply,
    DeviceTypeSecurity,
    DeviceTypeSoftwareDevice,
    DeviceTypeProcessor,
    DeviceTypeSystemDevices,
    DeviceTypeUSBController,
    DeviceTypeSensor,

    DeviceTypeUnknown
};

enum DeviceBus {
    DeviceBusSoftware = 0,
    DeviceBusPCI      = 1,
    DeviceBusUSB      = 2,
    DeviceBusPS2      = 3,

    DeviceBusUnknown = 4
};

class Device {
public:
    Device(const char* _name, DeviceBus _bus, DeviceType _type);
    Device(const char* _name)
        : Device(m_name, DeviceBusUnknown, DeviceTypeUnknown) {}
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

    virtual void enable()  = 0;
    virtual void disable() = 0;

protected:
    const char* m_name;
    u64         m_deviceId;
    u64         m_flags;

    DeviceType          m_type;
    DeviceBus           m_deviceBus;
    LinkedList<Device&> m_dependencies;
};