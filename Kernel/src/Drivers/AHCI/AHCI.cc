#include <Drivers/AHCI.h>
#include <Drivers/PCI.h>

namespace AHCI
{
    

    void Initialize()
    {
        using namespace PCI;

        PCIDeviceProvider *deviceProvider = static_cast<PCIDeviceProvider *>(g_DeviceProviders[DeviceBusPCI]);
        PCIInfo *deviceInfo = deviceProvider->FindGenericDevice(0x01, 0x06);

        if (Objects::IsNull(deviceInfo)) {
            return;
        }

        
    }
}