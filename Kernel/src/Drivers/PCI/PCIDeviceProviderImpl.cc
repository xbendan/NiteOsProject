#include <Device/Device.h>
#include <Drivers/PCI.h>
#include <Drivers/AHCI.h>
#include <system.h>

namespace PCI
{
    PCIDeviceProvider::PCIDeviceProvider()
    {
        m_DeviceList = LinkedList<PCIInfo>();
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
                    ConnectDevice(i, j, 0);

                    if (ConfigReadWord(i, j, 0, PCIHeaderType) & 0x80) {
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

    PCIInfo PCIDeviceProvider::ConnectDevice(uint8_t bus, uint8_t slot, uint8_t func)
    {
        PCIInfo deviceInfo = PCIInfo(bus, slot, func);
        m_DeviceList.Add(deviceInfo);
        return deviceInfo;
    }

    PCIInfo *PCIDeviceProvider::FindDevice(uint16_t deviceID, uint16_t vendorID)
    {
        
    }

    PCIInfo *PCIDeviceProvider::FindGenericDevice(uint16_t classCode, uint16_t subclass)
    {
        for (int i = 0; i < m_DeviceList.Count(); i++)
        {
            PCIInfo *deviceInfo = &m_DeviceList[i];
            if (deviceInfo->m_ClassCode == classCode && deviceInfo->m_SubClass == subclass)
                return deviceInfo;
        }

        return nullptr;
    }

    void PCIDeviceProvider::EnumerateDevice(uint16_t deviceID, uint16_t vendorID, void(*consumer)(PCIInfo& deviceInfo))
    {

    }

    void PCIDeviceProvider::EnumerateGenericDevice(uint8_t classCode, uint8_t subclass, void(*consumer)(PCIInfo& deviceInfo))
    {
        
    }

    // PCIDevice *PCIDeviceProvider::ConnectDevice(int bus, int slot, int func)
    // {
    //     uint8_t _classCode = ConfigReadByte(bus, slot, func, PCIClassCode);
    //     uint8_t _subclass = ConfigReadByte(bus, slot, func, PCISubclass);
    //     PCIDevice *device;

    //     switch (_classCode) {
    //         case 0x01 /* Mass Storage Controller */ : switch (_subclass) {
    //             case 0x06 /* Serial ATA Controller */ : device = new SerialATAController(); break;
    //             // case 0x08 /* Non-Volatile Memory Controller */: 
    //             default:
    //                 break;
    //             }
    //         case 0x04 /* Multimedia Controller */: switch (_subclass) {
    //             case 0x00 /* Multimedia Video Controller */: break;
    //             case 0x01 /* Multimedia Audio Controller */: break;
    //             default: device = new PCIDevice(bus, slot, func, _classCode, _subclass) break;
    //             }
    //     }
    //     return ConnectDevice(new PCIDevice(bus, slot, func));
    // }
}