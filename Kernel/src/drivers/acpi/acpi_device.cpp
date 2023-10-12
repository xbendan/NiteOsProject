#include <arch/x86_64/kaddr.h>
#include <common/logger.h>
#include <common/string.h>
#include <siberix/core/runtimes.h>
#include <siberix/drivers/acpi/acpi_device.h>
#include <siberix/drivers/acpi/acpi_timer.h>

AcpiPmDevice::AcpiPmDevice()
  : Device("ACPI Power Management")
{
    const char* signature = "RSD PTR ";

    u64  address    = 0;
    auto f_findRsdp = [&](u64 addrStart, u64 addrEnd) -> AcpiRsdp* {
        address = addrStart;
        while (address <= addrEnd) {
            if (memcmp((void*)IOVB(address), signature, 8) == 0) {
                return reinterpret_cast<AcpiRsdp*>(IOVB(address));
            }
            address += 0x10;
        }
        return 0;
    };
    rsdp = f_findRsdp(0x0, 0x7bff + 1);
    if (!rsdp) {
        rsdp = f_findRsdp(0x80000, 0x9ffff + 1);
        if (!rsdp) {
            rsdp = f_findRsdp(0xe0000, 0xfffff + 1);
        }
    }

    /*
     * Detect ACPI Root System Description Pointer (RSDP)
     * We cannot initialize ACPI without it.
     */
    if (!rsdp) {
        Logger::getLogger("acpi").error(
          "ACPI root system description pointer not found!\n");
        return;
    }

    /*
     * Handle ACPI Revision
     * When Revision
     *  ->  2           = ACPI Version 2.0 ~ 6.3
     *                       ACPI 2.0+ contains an extended structure so RSDP
     * needs to be "reinterpret_cast" as XSDP
     *  ->  0           = ACPI Version 1.0
     * If non of them above matches the revision, call kernel panic
     * because there shouldn't be any exceptions.
     */
    switch (rsdp->revision) {
        case 0:
            rsdt = reinterpret_cast<AcpiRsdt*>(IOVB(rsdp->rsdtAddress));
            break;
        case 2:
            rsdt = reinterpret_cast<AcpiRsdt*>(IOVB(rsdp->rsdtAddress));
            xsdt = reinterpret_cast<AcpiXsdt*>(IOVB(xsdp->xsdtAddress));
            break;
        default:
            Logger::getLogger("acpi").error("Invalid ACPI revision number %u\n",
                                            rsdp->revision);
            break;
    }

    madt = findTable<Madt>("APIC");
    fadt = findTable<AcpiFadt>("FACP");
    hpet = findTable<Hpet>("HPET");
    mcfg = findTable<PciMcfg>("MCFG");

    /* Initialize ACPI Timer */
    TimerDevice* device = new AcpiTimerDevice(fadt);
    if (device->isWorking()) {
        kern()->addTimer(device, true);
    } else {
        Logger::getLogger("acpi").error(
          "ACPI Timer ran into problem. Giving up installing.");
    }

    m_flags    |= DeviceInitialized;
    m_deviceId  = kern()->getConnectivity()->registerDevice(this);
};

AcpiPmDevice::~AcpiPmDevice() {}

template<typename T>
T*
AcpiPmDevice::findTable(const char* str, int index)
{
    if (!rsdp) {
        return nullptr;
    }

    if (memcmp("DSDT", str, 4) == 0)
        return reinterpret_cast<T*>(IOVB(fadt->dsdt));

    u64 entries = rsdp->revision
                    ? (xsdt->length - sizeof(AcpiTable)) / sizeof(u64)
                    : (rsdt->length - sizeof(AcpiTable)) / sizeof(u32);
    int _index  = 0;
    for (int i = 0; i < entries; i++) {
        u64 entry = rsdp->revision ? xsdt->pointers[i] : rsdt->pointers[i];
        AcpiTable* table = reinterpret_cast<AcpiTable*>(IOVB(entry));
        if (memcmp(table->signature, str, 4) == 0 && (_index++ == index)) {
            u8  sum     = 0;
            u8* pointer = reinterpret_cast<u8*>(table);
            for (u64 i = 0; i < table->length; i++) {
                sum += pointer[i];
            }
            Logger::getLogger("acpi").info(
              "The checksum of table %s is %u, given is %u\n",
              table->signature,
              sum & 0xff,
              table->checksum);
            return reinterpret_cast<T*>(table);
        }
    }
    return nullptr;
}