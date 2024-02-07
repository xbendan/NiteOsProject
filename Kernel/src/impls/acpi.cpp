#include <drivers/acpi/device.h>
#include <siberix/mem/mem.h>
#include <siberix/mem/range.h>
#include <siberix/panic.h>

namespace Kern::Hal::Impls {
    using namespace Specs;

    AcpiMgmtDevice::AcpiMgmtDevice()
      : Device("ACPI Power Management Device", DeviceType::Firmware)
    {
        const char* sign        = "RSD PTR ";
        uint64_t    rsdpAddress = Mem::copyAsIoAddress(
          Mem::rangeOf(0x0, 0x7bff + 1).find((uint8_t*)sign, 8, 0x10) |
          Mem::rangeOf(0x80000, 0x9ffff + 1).find((uint8_t*)sign, 8, 0x10) |
          Mem::rangeOf(0xe0000, 0xfffff + 1).find((uint8_t*)sign, 8, 0x10));
        /*
            Detect ACPI Root System Description Pointer (RSDP)
            We cannot initialize ACPI without it.
         */
        if (!rsdpAddress) {
            // TODO: Kernel panic
            callPanic("ACPI: RSDP not found");
            return;
        } else {
            m_rsdp = reinterpret_cast<AcpiRsdp*>(rsdpAddress);
        }

        /*
            Handle ACPI Revision
            When Revision
             ->  2           = ACPI Version 2.0 ~ 6.3
                               ACPI 2.0+ contains an extended structure so RSDP
                               needs to be cast into XSDP
             ->  0           = ACPI Version 1.0
            If non of them above matches the revision, call kernel panic
            because there shouldn't be any exceptions.
         */
        switch (m_rsdp->_revision) {
            case 0:
                m_rsdt = reinterpret_cast<AcpiRsdt*>(
                  Mem::copyAsIoAddress(m_rsdp->_rsdtAddress));
                break;
            case 2:
                m_xsdp = reinterpret_cast<AcpiXsdp*>(m_rsdp);

                m_rsdt = reinterpret_cast<AcpiRsdt*>(
                  Mem::copyAsIoAddress(m_rsdp->_rsdtAddress));
                m_xsdt = reinterpret_cast<AcpiXsdt*>(
                  Mem::copyAsIoAddress(m_xsdp->_xsdtAddress));
                break;
            default:
                break;
        }

        m_madt = findTable<Madt>("APIC");
        m_fadt = findTable<AcpiFadt>("FACP");
        m_hpet = findTable<Hpet>("HPET");
        m_mcfg = findTable<PciMcfg>("MCFG");

        // TODO: Initialize ACPI timer
    }

    AcpiMgmtDevice::~AcpiMgmtDevice() {}

    template <typename T>
        requires Std::isBaseOf<AcpiTable, T>::value
    T* AcpiMgmtDevice::findTable(Std::String<Utf8> name, int index)
    {
        if (!m_rsdp) {
            return nullptr;
        }

        if (name.equals("DSDT")) {
            return reinterpret_cast<T*>(Mem::copyAsIoAddress(m_fadt->_dsdt));
        }

        uint64_t entries =
          m_rsdp->_revision
            ? (m_xsdt->_length - sizeof(AcpiTable)) / sizeof(uint64_t)
            : (m_rsdt->_length - sizeof(AcpiTable)) / sizeof(uint32_t);
        int _index = 0;
        for (int i = 0; i < entries; i++) {
            uint64_t ent =
              m_rsdp->_revision ? m_xsdt->_pointers[i] : m_rsdt->_pointers[i];
            AcpiTable* table =
              reinterpret_cast<AcpiTable*>(Mem::copyAsIoAddress(ent));

            if (name.equals(table->_signature) && (_index++ == index)) {
                if (table->checksum() != 0) {
                    // TODO: Raise a warning
                }
                return reinterpret_cast<T*>(table);
            }
        }
        return nullptr;
    }
}