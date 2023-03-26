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

enum DeviceProperty
{

};

class DeviceProvider;
class DeviceController;

namespace Fs { struct File; }

class Device
{
private:
    static uint64_t allocatedId = 0;

    const char *m_Name;
    DeviceType m_Type;
    DeviceBus m_DeviceBus;
    uint64_t m_DeviceId;
    Fs::File *m_DriverSource;
    LinkedList<Device *> m_Dependencies; /* Device that this device depends on */
    DeviceController *m_Controller;
    DeviceProvider *m_Provider;
    uint64_t m_Properties;

public:
    static SizedArrayList<DeviceProvider *, 4> g_DeviceProviders;
    static SizedArrayList<LinkedList<Device *>, DeviceType::DeviceTypeUnknown + 1> g_DeviceLists;

    Device(const char *name, DeviceBus bus, DeviceType type);
    Device(const char *name) : Device(name, DeviceBus::DeviceBusUnknown, DeviceType::DeviceTypeUnknown) {}
    Device(DeviceBus bus, DeviceType type): Device(nullptr, bus, type) {}

    static Device GetDevice(const char *str);
    static Device GetDevice(uint64_t id);
    static void RegisterDevice(Device *device);
    static DeviceProvider GetProvider(DeviceBus bus);
    static void EnumerateDevice(DeviceType type);

    inline const char *Name() { return m_Name; }
    inline DeviceType Type() { return m_Type; }
    inline DeviceBus Bus() { return m_DeviceBus; }
    inline uint64_t Id() { return m_DeviceId; }
    inline DeviceProvider Provider() { return m_Provider; }
    inline LinkedList<Device *> &Dependencies() { return m_Dependencies; }
    
    inline bool IsPropertySet(DeviceProperty property) { return m_Properties & property; }
    inline bool IsDependent(Device *device) { return m_Dependencies.Contains(device); }

    inline void Register() { RegisterDevice(this); }

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
    void RegisterDevice()
    DeviceProvider *GetProvider(DeviceBus bus);
    void EnumerateDevice(DeviceType type);
}