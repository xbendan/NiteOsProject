#include <Drivers/Bus/PCI.h>
#include <Device/Device.h>

namespace PCI
{
    PCIDevice::PCIDevice(uint8_t bus, uint8_t slot, uint8_t func, uint8_t classCode, uint8_t subclass)
      : Device(DeviceBus::DeviceBusPCI, DeviceType::DeviceTypeUnknown),
        PCIInfo(bus, slot, func, classCode, subclass)
    {
        
    }

    PCIDevice::~PCIDevice()
    {

    }

    void PCIDevice::Enable()
    {

    }

    void PCIDevice::Disable()
    {
        
    }
}