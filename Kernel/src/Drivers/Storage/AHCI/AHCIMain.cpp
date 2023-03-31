#include <Drivers/Storage/AHCI.h>
#include <Drivers/Bus/PCI.h>

namespace AHCI
{
    AHCIControllerDevice *g_ControllerDevice;

    void Initialize()
    {
        using namespace PCI;

        DeviceProvider *deviceProvider = Device::g_DeviceProviders[DeviceBusPCI];
        PCIInfo *deviceInfo;

        if (Objects::IsNull(deviceProvider) || Objects::IsNull(deviceInfo = (static_cast<PCIDeviceProvider *>(deviceProvider))->FindGenericDevice(0x01, 0x06))) {
            System::Out("AHCI Controller not found.");
            return;
        }

        g_ControllerDevice = new AHCIControllerDevice(*deviceInfo);

        Objects::Validate(g_ControllerDevice->VendorID() != 0xFFFF,
            "AHCI Controller contains wrong vendor ID.");
    }
}