#include <Drivers/PCI.h>
#include <Device/Device.h>

namespace PCI
{
    PCIDevice::PCIDevice(uint8_t bus, uint8_t slot, uint8_t func)
      : m_Bus(bus),
        m_Slot(slot),
        m_Func(func),
        m_DeviceID(ConfigReadWord(bus, slot, func, PCIDeviceID)),
        m_VendorID(ConfigReadWord(bus, slot, func, PCIVendorID)),
        m_ClassCode(ConfigReadByte(bus, slot, func, PCIClassCode)),
        m_SubClass(ConfigReadByte(bus, slot, func, PCISubclass)),
        m_ProgIf(ConfigReadByte(bus, slot, func, PCIProgIF)), 
        Device(DeviceBus::DeviceBusPCI, DeviceType::DeviceTypeUnknown)
    {

    }

    PCIDevice::~PCIDevice()
    {

    }
}