#include <arch/x86_64/apic.hpp>
#include <common/logger.h>
#include <siberix/core/runtimes.hpp>
#include <siberix/device/types.hpp>
#include <siberix/drivers/acpi/acpi_device.hpp>

ApicManagementDevice::ApicManagementDevice() {
    AcpiPmDevice* acpiDevice;
    if (acpiDevice == nullptr)
    {
        Logger::getLogger("apic").error("ACPI device not detected! Stop loading APIC.");
        return;
    }

    u64 offset = acpiDevice->madt->entries;
    u64 end = reinterpret_cast<u64>(acpiDevice->madt) + acpiDevice->madt->length;
    while (offset < end)
    {
        MadtEntry* entry = reinterpret_cast<MadtEntry*>(offset);
        switch (entry->type)
        {
            case 0x00: /* Processor Local APIC */ {
                MadtLocalApic* apicLocal = static_cast<MadtLocalApic*>(entry);
                if ((apicLocal->flags & 0x3) && apicLocal->apicId)
                {
                    runtime()->getDeviceTree()->registerDevice(new ProcessorDevice(apicLocal->apicId));
                    _locals[apicLocal->apicId] = ApicLocalInterface(apicLocal->apicId);
                }
                break;
            }
            case 0x01: /* IO APIC */ {
                MadtIoApic* apicIo = static_cast<MadtIoApic*>(entry);
                if (!apicIo->gSiB)
                    basePhys = apicIo->address;
                break;
            }
            case 0x02: /* Interrupt Source Override */ {
                overrides.add(static_cast<MadtIso*>(entry) iso);
                break;
            }
            case 0x03: /* Non-maskable Interrupt */ {
                MadtNmi* nmi = static_cast<MadtNmi*>(entry);
                break;
            }
            case 0x04: /* Local APIC Non-maskable Interrupt */ {
            }
            case 0x05: /* Local APIC Address Override */ {
            }
            case 0x09: /* Processor Local x2APIC */ {
                break;
            }
            case 0xa: /* Local x2APIC Non-maskable Interrupt */ {
                break;
            }
            case 0xb: /*  */ {
                break;
            }

            default: break;
        }
        offset += entry->length;
    }

    if (!basePhys)
    {
        Logger::getLogger("apic").error("I/O APIC base address not found!");
        return;
    }

    baseVirtIO = getIoMapping(basePhys);
    ioRegSelect = (u32*)(baseVirtIO + IO_APIC_REGSEL);
    ioWindow = (u32*)(baseVirtIO + IO_APIC_WIN);

    for (int i = 0; i < overrides.length(); i++)
    {
        MadtIso* iso = overrides[i];
        ioWrite64(IO_APIC_RED_TABLE_ENT(iso->gSi), iso->irqSource + 0x20);
    }
}

ApicManagementDevice::~ApicManagementDevice() {}

void ApicManagementDevice::ioWrite(u32 reg, u32 data) {}

u32 ApicManagementDevice::ioRead64(u32 reg) {}

void ApicManagementDevice::ioWrite64(u32 reg, u64 data) {}

u64 ApicManagementDevice::ioRead64(u32 reg) {}

void ApicManagementDevice::lWriteBase(u64 val) {}

void ApicManagementDevice::lReadBase() {}