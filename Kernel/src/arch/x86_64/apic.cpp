#include <arch/x86_64/apic.h>
#include <arch/x86_64/kaddr.h>
#include <common/logger.h>
#include <siberix/core/runtimes.h>
#include <siberix/device/types.h>
#include <siberix/drivers/acpi/acpi_device.h>

ApicLocalInterface            ApicDevice::m_interfaces[256];
SizedArrayList<MadtIso*, 256> ApicDevice::m_overrides;

ApicTimerDevice::ApicTimerDevice(ApicLocalInterface& interface)
    : TimerDevice("APIC Timer"),
      m_busClock(0),
      m_ticks(0) {}

ApicTimerDevice::~ApicTimerDevice() {}

void ApicTimerDevice::sleep(Duration duration) {}

void ApicTimerDevice::sleep(u64 ms) {}

ApicDevice::ApicDevice()
    : Device("Advanced Programmable Interrupt Controller") {
    AcpiPmDevice* acpiDevice;
    if (acpiDevice == nullptr) {
        Logger::getLogger("apic").error("ACPI device not detected! Stop loading APIC.");
        return;
    }

    u64 offset = (u64) & (acpiDevice->madt->entries[0]);
    u64 end    = reinterpret_cast<u64>(acpiDevice->madt) + acpiDevice->madt->length;
    while (offset < end) {
        MadtEntry* entry = reinterpret_cast<MadtEntry*>(offset);
        switch (entry->type) {
            case 0x00: /* Processor Local APIC */ {
                MadtLocalApic* apicLocal = static_cast<MadtLocalApic*>(entry);
                if ((apicLocal->flags & 0x3) && apicLocal->apicId) {
                    siberix()->getConnectivity()->registerDevice(
                        new ProcessorDevice(apicLocal->apicId));
                    m_interfaces[apicLocal->apicId] = ApicLocalInterface(apicLocal->apicId, this);
                }
                break;
            }
            case 0x01: /* IO APIC */ {
                MadtIoApic* apicIo = static_cast<MadtIoApic*>(entry);
                if (!apicIo->gSiB) basePhys = apicIo->address;
                break;
            }
            case 0x02: /* Interrupt Source Override */ {
                m_overrides.add(static_cast<MadtIso*>(entry));
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

            default:
                break;
        }
        offset += entry->length;
    }

    if (!basePhys) {
        Logger::getLogger("apic").error("I/O APIC base address not found!");
        return;
    }

    baseVirtIO  = IOVB(basePhys);
    ioRegSelect = (u32*)(baseVirtIO + IO_APIC_REGSEL);
    ioWindow    = (u32*)(baseVirtIO + IO_APIC_WIN);

    for (int i = 0; i < m_overrides.length(); i++) {
        MadtIso* iso = m_overrides[i];
        ioWrite64(IO_APIC_RED_TABLE_ENT(iso->gSi), iso->irqSource + 0x20);
    }
}

ApicDevice::~ApicDevice() {}

void ApicDevice::ioWrite(u32 reg, u32 data) {
    *ioRegSelect = reg;
    *ioWindow    = data;
}

u32 ApicDevice::ioRead(u32 reg) {
    *ioRegSelect = reg;
    return *ioWindow;
}

void ApicDevice::ioWrite64(u32 reg, u64 data) {
    u32 low = (data & 0xffffffff), high = (data >> 32);
    ioWrite(reg, low);
    ioWrite(reg + 1, high);
}

u64 ApicDevice::ioRead64(u32 reg) {
    u32 low = ioRead(reg), high = ioRead(reg + 1);
    return low | ((u64)high << 32);
}

void ApicDevice::lWriteBase(u64 val) {
    u64 low  = (val & 0xffffffff) | 0x800;
    u64 high = val >> 32;
    asm("wrmsr" ::"a"(low), "d"(high), "c"(0x1b));
}

u64 ApicDevice::lReadBase() {
    u64 low, high;
    asm("rdmsr" : "=a"(low), "=d"(high) : "c"(0x1b));
    return ((high & 0x0f) << 32) | (low & 0xffffffff);
}