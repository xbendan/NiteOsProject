#include <utils/list.h>

enum DeviceType
{
    DeviceTypeAudio,
    DeviceTypeBattery,
    DeviceTypeBluetooth,
    DeviceTypeFsDriveDevice,
    DeviceTypeFsDriveController,
    DeviceTypeDisplayAdapter,
    DeviceTypeDisplayMonitor,
    DeviceTypeUSBDevice,
    DeviceTypeUSBController,
    DeviceTypePCIDevice,
    DeviceTypePCIController,
    DeviceTypeNetworkAdapter,
    DeviceTypeSecurity,
    DeviceTypeProcessor,
    DeviceTypePrinter,
    DeviceTypeSystemDevices,

    DeviceTypeUnknown
};

enum DeviceBus
{
    DeviceBusSoftware,
    DeviceBusPCI,
    DeviceBusUSB,

    DeviceBusUnknown
};

class Device
{
    const char *m_Name;
    DeviceType m_Type;
    DeviceBus m_DeviceBus;
    uint64_t m_DeviceId;

    LinkedList<Device *> m_Dependencies; /* Device that this device depends on */

public:
    Device(const char *name)
      : m_Name(name),
        m_Type() { }
};

Device *GetDevice(const char *str);
