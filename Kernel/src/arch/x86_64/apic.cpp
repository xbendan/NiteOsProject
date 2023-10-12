#include <arch/x86_64/apic.h>
#include <arch/x86_64/kaddr.h>
#include <common/logger.h>
#include <siberix/core/runtimes.h>
#include <siberix/device/types.h>
#include <siberix/drivers/acpi/acpi_device.h>

ApicLocal            ApicDevice::m_interfaces[256];
SizedArrayList<MadtIso*, 256> ApicDevice::m_overrides;

ApicDevice::ApicDevice()
  : Device("Advanced Programmable Interrupt Controller")
{
    AcpiPmDevice* acpiDevice = static_cast<AcpiPmDevice*>(
      kern()->getConnectivity()->findDevice("ACPI Power Management"));
    if (acpiDevice == nullptr) {
        Logger::getLogger("apic").warn(
          "ACPI device not detected! Stop loading APIC.");
        return;
    }

    u64 offset = (u64) & (acpiDevice->madt->entries[0]);
    u64 end =
      reinterpret_cast<u64>(acpiDevice->madt) + acpiDevice->madt->length;
    while (offset < end) {
        MadtEntry* entry = reinterpret_cast<MadtEntry*>(offset);
        switch (entry->type) {
            case 0x00: /* Processor Local APIC */ {
                MadtLocalApic* apicLocal = static_cast<MadtLocalApic*>(entry);
                if (apicLocal->flags & 0x3) {
                    kern()->getConnectivity()->registerDevice(
                      new ProcessorDevice(apicLocal->apicId));

                    m_interfaces[apicLocal->apicId] =
                      ApicLocal(apicLocal->apicId, this);
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
                m_overrides.add(static_cast<MadtIso*>(entry));
                break;
            }
            case 0x03: /* Non-maskable Interrupt */ {
                MadtNmi* nmi = static_cast<MadtNmi*>(entry);
                Logger::getLogger("apic").info(
                  "Non-maskable Interrupt detected: %u", nmi->lInt);
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

            default:
                break;
        }
        offset += entry->length;
    }

    if (!basePhys) {
        Logger::getLogger("apic").warn("I/O APIC base address not found!");
        return;
    }

    baseVirtIO  = IOVB(basePhys);
    ioRegSelect = (u32*)(baseVirtIO + IO_APIC_REGSEL);
    ioWindow    = (u32*)(baseVirtIO + IO_APIC_WIN);
    interrupts  = ioRead(IO_APIC_REGISTER_VER) >> 16;
    // lWriteBase(lReadBase() | (1UL << 11));

    Logger::getLogger("apic").info("APIC initialized with base address %x.\n",
                                   basePhys);

    m_interfaces[0].setup();

    for (int i = 0; i < m_overrides.length(); i++) {
        MadtIso* iso = m_overrides[i];
        ioWrite64(IO_APIC_RED_TABLE_ENT(iso->gSi), iso->irqSource + 0x20);
        Logger::getLogger("apic").info(
          "Remapped IRQ from [%u] to [%u]\n", iso->gSi, iso->irqSource + 0x20);
    }

    if ((readMSR(0x1b)) != 0) {
        m_flags |= DeviceFlags::DeviceInitialized;
    }

    kern()->getConnectivity()->registerDevice(this);
}

ApicDevice::~ApicDevice() {}

void
ApicDevice::enable()
{
}

void
ApicDevice::disable()
{
}

void
ApicDevice::ioWrite(u32 reg, u32 data)
{
    *ioRegSelect = reg;
    *ioWindow    = data;
}

u32
ApicDevice::ioRead(u32 reg)
{
    *ioRegSelect = reg;
    return *ioWindow;
}

void
ApicDevice::ioWrite64(u32 reg, u64 data)
{
    u32 low = (data & 0xffffffff), high = (data >> 32);
    ioWrite(reg, low);
    ioWrite(reg + 1, high);
}

u64
ApicDevice::ioRead64(u32 reg)
{
    u32 low = ioRead(reg), high = ioRead(reg + 1);
    return low | ((u64)high << 32);
}

void
ApicDevice::lWriteBase(u64 val)
{
    u64 low  = (val & 0xffffffff) | 0x800;
    u64 high = val >> 32;
    asm("wrmsr" ::"a"(low), "d"(high), "c"(0x1b));
}

u64
ApicDevice::lReadBase()
{
    u64 low, high;
    asm("rdmsr" : "=a"(low), "=d"(high) : "c"(0x1b));
    return ((high & 0x0f) << 32) | (low & 0xffffffff);
}