#include <siberix/drivers/pci/devices.h>

PciDevice::PciDevice(u8 bus, u8 slot, u8 func)
  : Device("Unnamed PCI Device", DeviceBus::PCI, DeviceType::Unknown)
  , m_info(PCIInfo(bus, slot, func))
{
}

PciDevice::PciDevice(u8 bus, u8 slot, u8 func, u8 classCode, u8 subClass)
  : Device("Unnamed PCI Device", DeviceBus::PCI, DeviceType::Unknown)
  , m_info(PCIInfo(bus, slot, func, classCode, subClass))
{
}

PciDevice::PciDevice(PCIInfo& info)
  : Device("Unnamed PCI Device", DeviceBus::PCI, DeviceType::Unknown)
  , m_info(info)
{
}

PciDevice::~PciDevice() {}