#include <common/logger.h>

#include <siberix/core/runtimes.hpp>
#include <siberix/device/connectivity.hpp>
#include <siberix/drivers/acpi/acpi_device.hpp>
#include <siberix/drivers/pci/devices.hpp>

PciControllerDevice::PciControllerDevice() {
    m_deviceList          = LinkedList<PCIInfo>();
    m_enhancedAddressList = LinkedList<McfgAddress*>();

    AcpiPmDevice* acpiDevice =
        runtime()->getConnectivity().findDevice("ACPI Power Management");
    if (acpiDevice == nullptr) {
        Logger::getLogger("pci").error(
            "No ACPI Device found. Abort to load PCI controller.");
        m_flags |= (DeviceFlags::Exception);
        return;
    } else {
        m_mcfgTable = acpiDevice->findTable("MCFG");
        if (m_mcfgTable == nullptr) {
            Logger::getLogger("pci").error(
                "No PCI MCFG table found. Abort to load PCI controller.");
            return;
        }
    }

    m_accessMode = PCIConfigAccessMode::Enhanced;
    for (unsigned i = 0;
         i < (m_mcfgTable->length - sizeof(PciMcfg)) / sizeof(McfgAddress);
         i++) {
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

bool PciControllerDevice::checkDevice(u8 bus, u8 device, u8 func) {
    return !(getVendorID(bus, device, func) == 0xffff);
}

PciDevice& PciControllerDevice::connectDevice(u8 bus, u8 slot, u8 func) {
    PciDevice* device = new PciDevice(bus, slot, func);
    m_deviceList.add(*device);
    return *device;
}

PciDevice* PciControllerDevice::findDevice(u16 deviceID, u16 vendorID) {}

PciDevice* PciControllerDevice::findGenericDevice(u16 classCode, u16 subclass) {
}

void PciControllerDevice::enumerateDevice(u16 deviceID,
                                          u16 vendorID,
                                          void (*consumer)(PciDevice& device)) {
}

void PciControllerDevice::enumerateGenericDevice(
    u8 classCode, u8 subclass, void (*consumer)(PciDevice& device)) {}