#include <Drivers/PCI.h>
#include <Device/Device.h>

namespace PCI
{
    PCIDevice::PCIDevice(uint8_t bus, uint8_t slot, uint8_t func, uint16_t classCode, uint16_t subclass)
      : Device(DeviceBus::DeviceBusPCI, DeviceType::DeviceTypeUnknown),
        PCIInfo(bus, slot, func, classCode, subclass)
    {

    }

    PCIDevice::~PCIDevice()
    {

    }
}