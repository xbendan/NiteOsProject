#include <drivers/pci/device.h>
#include <siberix/dvc/device-host.h>
#include <stdcxx/linked-list.h>

namespace PCI {
    class PCIDeviceEnumerator : public Kern::DeviceEnumerator
    {
    public:
        PCIDeviceEnumerator(Kern::DeviceConnectivity* deviceHost)
          : Kern::DeviceEnumerator("PCI Device Enumerator")
          , m_deviceHost(deviceHost)
        {
        }
        ~PCIDeviceEnumerator();

        Std::Array<Kern::IDevice&>& scan() override;
        bool     checkDevice(uint8_t bus, uint8_t slot, uint8_t func);
        bool     checkDevice(PCIInfo& info);
        PCIInfo* getDevice(uint16_t deviceId, uint16_t vendorId);
        PCIInfo* getDevice(uint8_t classCode, uint8_t subclassCode);

    private:
        Kern::DeviceConnectivity*           m_deviceHost;
        Std::LinkedList<PCIDevice&> m_devices;
    };
}