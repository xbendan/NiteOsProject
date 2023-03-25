#pragma once

#include <Utils/LinkedList.h>
#include <Utils/ArrayList.h>

enum DeviceType
{
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

enum DeviceBus
{
    DeviceBusSoftware = 0,
    DeviceBusPCI = 1,
    DeviceBusUSB = 2,
    DeviceBusPS2 = 3,

    DeviceBusUnknown = 4
};

class DeviceProvider;
class DeviceController;

namespace Fs { struct File; }

class Device
{
public:
    const char *m_Name;
    DeviceType m_Type;
    DeviceBus m_DeviceBus;
    uint64_t m_DeviceId;
    Fs::File *m_DriverSource;

    DeviceController *m_Controller;
    DeviceProvider *m_Provider;
    struct
    {
        bool AllowDisableToSavePower : 1;
    };

    LinkedList<Device *> m_Dependencies; /* Device that this device depends on */

    Device(const char *name, DeviceBus bus, DeviceType type);
    Device(const char *name) : Device(name, DeviceBus::DeviceBusUnknown, DeviceType::DeviceTypeUnknown) {}
    Device(DeviceBus bus, DeviceType type): Device(nullptr, bus, type) {}

    virtual void Enable() = 0;
    virtual void Disable() = 0;
};

class DeviceProvider
{
public:
    DeviceBus m_DeviceBus;

    virtual Device *FindName(const char *str);
};

namespace DeviceManagement
{
    extern SizedArrayList<DeviceProvider *, 4> g_DeviceProviders;
    extern SizedArrayList<LinkedList<Device *>, DeviceType::DeviceTypeUnknown + 1> g_DeviceLists;

    Device *GetDevice(const char *str);
    Device *GetDevice(uint64_t id);
    DeviceProvider *GetProvider(DeviceBus bus);
    void EnumerateDevice(DeviceType type);
}