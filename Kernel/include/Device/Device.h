#pragma once

#include <Utils/LinkedList.h>
#include <Utils/ArrayList.h>

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
    DeviceBusSoftware = 0,
    DeviceBusPCI = 1,
    DeviceBusUSB = 2,

    DeviceBusUnknown = 3
};

class DeviceProvider;

namespace Fs { struct File; }

class Device
{
public:
    const char *m_Name;
    DeviceType m_Type;
    DeviceBus m_DeviceBus;
    uint64_t m_DeviceId;
    Fs::File *m_DriverSource;

    DeviceProvider *m_Provider;
    struct
    {
        bool AllowDisableToSavePower : 1;
    };

    LinkedList<Device *> m_Dependencies; /* Device that this device depends on */

    Device(const char *name, DeviceBus bus, DeviceType type);
    Device(const char *name) : Device(name, DeviceBus::DeviceBusUnknown, DeviceType::DeviceTypeUnknown) {}
    Device(DeviceBus bus, DeviceType type): Device(nullptr, bus, type) {}

    // virtual void Disconnect();
    // virtual void Enable();
    // virtual void Disable();
};

class DeviceProvider
{
public:
    DeviceBus m_DeviceBus;

    virtual Device *FindName(const char *str);
};

Device *GetDevice(const char *str);
Device *GetDevice(uint64_t id);
DeviceProvider *GetProvider(DeviceBus bus);
Device **EnumerateDevice(DeviceType type);

extern SizedArrayList<DeviceProvider *, 4> g_DeviceProviders;