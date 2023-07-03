#include <common/typedefs.h>
#include <utils/linked_list.h>

extern char[16] _unknownDeviceName;

enum DeviceType {
    DeviceTypeBattery,
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
    DeviceTypeSecurity,
    DeviceTypeSoftwareDevice,
    DeviceTypeProcessor,
    DeviceTypeSystemDevices,
    DeviceTypeUSBController,

    DeviceTypeUnknown
};

enum DeviceBus {
    DeviceBusSoftware = 0,
    DeviceBusPCI = 1,
    DeviceBusUSB = 2,
    DeviceBusPS2 = 3,

    DeviceBusUnknown = 4
};

class Device {
public:
    Device(const char* _name, DeviceBus _bus, DeviceType _type);
    Device(const char* _name) : Device(name, DeviceBusUnknown, DeviceTypeUnknown) {}
    Device(DeviceBus bus, DeviceType type) : Device(&_unknownDeviceName, bus, type) {}

    inline const char* getName() { return this->name; }
    inline u64 getId() { return this->deviceId; }
    inline DeviceType getType() { return this->type; }
    inline DeviceBus getBus() { return this->bus; }
    inline LinkedList<Device&>& getDependencies() { return this->dependencies; }
    inline bool isDependent(Device& device) { return dependencies.contains(device); }

    virtual void Enable() = 0;
    virtual void Disable() = 0;

protected:
    const char* name;
    u64 deviceId;
    u64 flags;

    DeviceType type;
    DeviceBus bus;
    LinkedList<Device&> dependencies;
};