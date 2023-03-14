#include <Device/Device.h>
#include <Drivers/PCI.h>
#include <system.h>

namespace PCI
{
    PCIDeviceProvider::PCIDeviceProvider()
    {
        m_DeviceList = LinkedList<PCIDevice *>();
        m_EnhancedBaseAddressList = LinkedList<PciMcfgBaseAddress *>();

        m_McfgTable = ACPI::pciMcfg;
        if (m_McfgTable) 
        {
            m_AccessMode = ConfigurationAccessMode::Enhanced;
            for (unsigned i = 0; i < (m_McfgTable->length - sizeof(PciMcfg)) / sizeof(PciMcfgBaseAddress); i++)
            {
                PciMcfgBaseAddress *base = &m_McfgTable->BaseAddresses[i];
                if (base->SegmentGroupNumber)
                {
                    System::Out("No support for PCI express segments %u", base->SegmentGroupNumber);
                    continue;
                }

                m_EnhancedBaseAddressList.Add(base);
            }
        }

        for (uint16_t i = 0; i < 256; i++)
        {
            for (uint16_t j = 0; j < 32; j++)
            {
                if (CheckDevice(i, j, 0))
                {
                    PCIDevice *device = ConnectDevice(i, j, 0);

                    if (device->HeaderType() & 0x80) {
                        for (int k = 1; k < 8; k++) { // Func
                            if (CheckDevice(i, j, k)) {
                                ConnectDevice(i, j, k);
                            }
                        }
                    }
                }
            }
        }
    }

    PCIDeviceProvider::~PCIDeviceProvider()
    {

    }

    Device *PCIDeviceProvider::FindName(const char *str)
    {
        
    }

    PCIDevice *PCIDeviceProvider::ConnectDevice(PCIDevice *device)
    {
        m_DeviceList.Add(device);
        return device;
    }

    PCIDevice *PCIDeviceProvider::ConnectDevice(int bus, int slot, int func)
    {
        PCIDevice *device = new PCIDevice(bus, slot, func);
        return ConnectDevice(device);
    }
}