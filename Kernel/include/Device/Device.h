#include <Fs/VirtualFs.h>
#include <Utils/LinkedList.h>

namespace Device
{
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

    class DeviceProvider;
    class DeviceController;

    class Device
    {
        const char *m_Name;
        DeviceType m_Type;
        DeviceBus m_DeviceBus;
        uint64_t m_DeviceId;
        Fs::File m_DriverSource;
        
        DeviceProvider *m_Provider;
        DeviceController *m_Controller;

        struct 
        {
            bool AllowDisableToSavePower: 1;
        };

        LinkedList<Device *> m_Dependencies; /* Device that this device depends on */

    public:
        Device(const char *name)
          : m_Name(name),
            m_Type() { }

        virtual void Disconnect();
        virtual void Enable();
        virtual void Disable();
    };

    class DeviceProvider
    {
        DeviceBus m_DeviceBus;

        virtual Device *FindName(const char *str);
    };

    Device *GetDevice(const char *str);
    Device *GetDevice(uint64_t id);
    DeviceProvider *GetProvider(DeviceBus bus);
    Device **EnumerateDevice(DeviceType type);

    bool Connect(Device *device);
} // namespace Device
