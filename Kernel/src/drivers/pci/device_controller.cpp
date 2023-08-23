#include <arch/x86_64/iopt.h>
#include <common/logger.h>
#include <siberix/core/runtimes.h>
#include <siberix/device/connectivity.h>
#include <siberix/drivers/acpi/acpi_device.h>
#include <siberix/drivers/pci/devices.h>

PciControllerDevice::PciControllerDevice()
    : Device("PCI Controller", DeviceBus::PCI, DeviceType::Firmware) {
    m_deviceList          = LinkedList<PCIDevice&>();
    m_enhancedAddressList = LinkedList<McfgAddress*>();

    AcpiPmDevice* acpiDevice = reinterpret_cast<AcpiPmDevice*>(
        siberix()->getConnectivity()->findDevice("ACPI Power Management"));
    if (acpiDevice == nullptr) {
        Logger::getLogger("pci").error("No ACPI Device found. Abort to load PCI controller.");
        m_flags |= (DeviceExceptionOccurred);
        return;
    } else {
        m_mcfgTable = acpiDevice->findTable<PciMcfg>("MCFG");
        if (m_mcfgTable == nullptr) {
            Logger::getLogger("pci").error(
                "No PCI MCFG table found. Abort to load PCI controller.");
            return;
        }
    }

    m_accessMode = PCIConfigAccessMode::Enhanced;
    for (unsigned i = 0; i < (m_mcfgTable->length - sizeof(PciMcfg)) / sizeof(McfgAddress); i++) {
        McfgAddress* base = &m_mcfgTable->baseAddresses[i];
        if (base->sgn) {
        }

        m_enhancedAddressList.add(base);
    }

    for (u16 i = 0; i < 256; i++) {
        for (u16 j = 0; j < 32; j++) {
            if (checkDevice(i, j, 0)) {
                connectDevice(i, j, 0);
                if (configReadWord(i, j, 0, PCIHeaderType) & 0x80) {
                    for (int k = 1; k < 8; k++) {
                        if (checkDevice(i, j, k)) {
                            connectDevice(i, j, k);
                        }
                    }
                }
            }
        }
    }
}

PciControllerDevice::~PciControllerDevice() {}

u8 PciControllerDevice::configReadByte(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg) {
    outDWord32(0xcf8, PCI_PACKAGE_ADDRESS(bus, slot, func, reg));
    return (u8)((inDWord32(0xcfc) >> (reg & 2) * 8) & 0xff);
}

u16 PciControllerDevice::configReadWord(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg) {
    outDWord32(0xcf8, PCI_PACKAGE_ADDRESS(bus, slot, func, reg));
    return (u16)((inDWord32(0xcfc) >> (reg & 2) * 8) & 0xff);
}

u32 PciControllerDevice::configReadDWord(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg) {
    outDWord32(0xcf8, PCI_PACKAGE_ADDRESS(bus, slot, func, reg));
    return inDWord32(0xcfc);
}

void PciControllerDevice::configWriteByte(
    u8 bus, u8 slot, u8 func, PCIConfigRegisters reg, u8 data) {
    outDWord32(0xcf8, PCI_PACKAGE_ADDRESS(bus, slot, func, reg));
    outDWord32(0xcfc,
               (inDWord32(0xcfc) & (~(0xff << ((reg & 3) * 8)))) |
                   (static_cast<u32>(data) << ((reg & 3) * 8)));
}

void PciControllerDevice::configWriteWord(
    u8 bus, u8 slot, u8 func, PCIConfigRegisters reg, u16 data) {
    outDWord32(0xcf8, PCI_PACKAGE_ADDRESS(bus, slot, func, reg));
    outDWord32(0xcfc,
               (inDWord32(0xcfc) & (~(0xffff << ((reg & 2) * 8)))) |
                   (static_cast<u32>(data) << ((reg & 3) * 8)));
}

void PciControllerDevice::configWriteDWord(
    u8 bus, u8 slot, u8 func, PCIConfigRegisters reg, u32 data) {
    outDWord32(0xcf8, PCI_PACKAGE_ADDRESS(bus, slot, func, reg));
    outDWord32(0xcfc, data);
}

bool PciControllerDevice::checkDevice(u8 bus, u8 device, u8 func) {
    return !(getVendorID(bus, device, func) == 0xffff);
}

PCIDevice& PciControllerDevice::connectDevice(u8 bus, u8 slot, u8 func) {
    PCIDevice* device = new PCIDevice(bus, slot, func);
    m_deviceList.add(*device);
    return *device;
}

PCIDevice* PciControllerDevice::findDevice(u16 deviceID, u16 vendorID) {
    for (u32 i = 0; i < m_deviceList.count(); i++) {
        PCIDevice& device = m_deviceList[i];
        if (device.getDeviceID() == deviceID && device.getVendorID() == vendorID) {
            return &device;
        }
    }
    return nullptr;
}

PCIDevice* PciControllerDevice::findGenericDevice(u16 classCode, u16 subclass) {
    for (u32 i = 0; i < m_deviceList.count(); i++) {
        PCIDevice& device = m_deviceList[i];
        if (device.getClassCode() == classCode && device.getSubclass() == subclass) {
            return &device;
        }
    }
    return nullptr;
}

void PciControllerDevice::enumerateDevice(u16 deviceID,
                                          u16 vendorID,
                                          void (*consumer)(PCIDevice& device)) {
    ListNode<PCIDevice&>* node = m_deviceList.first();
    while (node != nullptr) {
        PCIDevice& device = node->obj;
        if (device.getDeviceID() == deviceID && device.getVendorID() == vendorID) {
            consumer(device);
        }
        node = node->next;
    }
}

void PciControllerDevice::enumerateGenericDevice(u8 classCode,
                                                 u8 subclass,
                                                 void (*consumer)(PCIDevice& device)) {
    ListNode<PCIDevice&>* node = m_deviceList.first();
    while (node != nullptr) {
        PCIDevice& device = node->obj;
        if (device.getClassCode() == classCode && device.getSubclass() == subclass) {
            consumer(device);
        }
    }
}