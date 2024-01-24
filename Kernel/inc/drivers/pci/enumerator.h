#include <drivers/pci/device.h>
#include <siberix/dvc/connectivity.h>
#include <stdcxx/linked-list.h>

namespace Kern::Hal::Impls {
    class PeriCompDeviceEnumerator : public Kern::IDeviceEnumerator
    {
    public:
        PeriCompDeviceEnumerator()
          : Kern::IDeviceEnumerator("PCI Device Enumerator")
        {
        }
        ~PeriCompDeviceEnumerator();

        Std::ArrayList<Device*>* scan() override;
        bool     checkDevice(uint8_t bus, uint8_t slot, uint8_t func);
        bool     checkDevice(PCIInfo& info);
        PCIInfo* getDevice(uint16_t deviceId, uint16_t vendorId);
        PCIInfo* getDevice(uint8_t classCode, uint8_t subclassCode);

    private:
        Kern::DeviceConnectivity*  m_deviceHost;
        Std::LinkedList<PCIDevice> m_devices;
    };
}