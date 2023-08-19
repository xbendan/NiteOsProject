#include <common/logger.h>
#include <siberix/core/runtimes.h>
#include <siberix/device/connectivity.h>
#include <siberix/drivers/acpi/acpi_device.h>
#include <siberix/drivers/pci/devices.h>

PCIControllerDevice::PCIControllerDevice()
    : Device("PCI Controller", DeviceBus::PCI, DeviceType::Firmware) {
    m_deviceList          = LinkedList<PCIDevice&>();
    m_enhancedAddressList = LinkedList<McfgAddress*>();

    AcpiPmDevice* acpiDevice = reinterpret_cast<AcpiPmDevice*>(
        exec()->getConnectivity()->findDevice("ACPI Power Management"));
    if (acpiDevice == nullptr) {
        Logger::getLogger("pci").error("No ACPI Device found. Abort to load PCI controller.");
        m_flags |= (DeviceFlags::Exception);
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

PCIControllerDevice::~PCIControllerDevice() {}

bool PCIControllerDevice::checkDevice(u8 bus, u8 device, u8 func) {
    return !(getVendorID(bus, device, func) == 0xffff);
}

PCIDevice& PCIControllerDevice::connectDevice(u8 bus, u8 slot, u8 func) {
    PCIDevice* device = new PCIDevice(bus, slot, func);
    m_deviceList.add(*device);
    return *device;
}

PCIDevice* PCIControllerDevice::findDevice(u16 deviceID, u16 vendorID) {
    for (u32 i = 0; i < m_deviceList.count(); i++) {
        PCIDevice& device = m_deviceList[i];
        if (device.getDeviceID() == deviceID && device.getVendorID() == vendorID) {
            return &device;
        }
    }
    return nullptr;
}

PCIDevice* PCIControllerDevice::findGenericDevice(u16 classCode, u16 subclass) {
    for (u32 i = 0; i < m_deviceList.count(); i++) {
        PCIDevice& device = m_deviceList[i];
        if (device.getClassCode() == classCode && device.getSubclass() == subclass) {
            return &device;
        }
    }
    return nullptr;
}

void PCIControllerDevice::enumerateDevice(u16 deviceID,
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

void PCIControllerDevice::enumerateGenericDevice(u8 classCode,
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