#include <arch/x86_64/iopt.h>
#include <arch/x86_64/kaddr.h>
#include <common/logger.h>
#include <siberix/core/runtimes.h>
#include <siberix/drivers/acpi/acpi_device.h>
#include <siberix/drivers/acpi/acpi_timer.h>

u32 ACPI_TIMER_READ_XMMIO(u32 data) { return *(u32*)(IOVB(data)); }

u32 ACPI_TIMER_READ_XIO(u32 data) { return inDWord32(data); }

u32 ACPI_TIMER_READ_IO(u32 data) { return inDWord32(data); }

AcpiTimerDevice::AcpiTimerDevice()
    : TimerDevice("ACPI Timer") {
    Device* device = exec()->getConnectivity()->findDevice("ACPI Power Management");
    if (device == nullptr) {
        Logger::getLogger("acpi").error(
            "ACPI Power Management device not detected or not installed.");
        return;
    }
    AcpiFadt* fadt = static_cast<AcpiPmDevice*>(device)->fadt;
    if (fadt == nullptr) {
        Logger::getLogger("acpi").error("Fixed ACPI Description Table not found.");
        return;
    }

    this->m_timerLength = fadt->pmtTimerLength;
    this->m_xpmtAddress = &fadt->x_pmtTimerBlock;
    this->m_is32bitMode = (fadt->flags & (1 << 7));
    switch (m_xpmtAddress->addressSpace) {
        case 0: /* Memory */ {
            m_reader = ACPI_TIMER_READ_XMMIO;
            m_data   = m_xpmtAddress->address;
        }
        case 1: /* IO */ {
            m_reader = ACPI_TIMER_READ_XIO;
            m_data   = m_xpmtAddress->address;
        }
        default: /* Unsupported */ {
            Logger::getLogger("acpi").warn(
                "Advanced ACPI Timer is not supported! Go back to eariler "
                "version.");
            m_reader = ACPI_TIMER_READ_IO;
            m_data   = fadt->pmtTimerBlock;
        }
    }

    m_flags |= DeviceFlags::Initialized;
}

AcpiTimerDevice::~AcpiTimerDevice() {}

void AcpiTimerDevice::sleep(Duration duration) {
    u64 ms = duration.amount;
    switch (duration.span) {
        case TimeSpan::Millisecond:
            break;
        case TimeSpan::Microsecond: {
            ms = (ms <= 1000 ? 1 : ms / 1000);
            break;
        }
        case TimeSpan::Nanosecond: {
            ms = (ms <= 1000000 ? 1 : ms / 1000000);
            break;
        }
        case TimeSpan::Second: {
            ms *= 1000;
            break;
        }
        case TimeSpan::Minute: {
            ms *= (1000 * 60);
            break;
        }
        case TimeSpan::Hour: {
            ms *= (1000 * 60 * 60);
            break;
        }
        case TimeSpan::Day: {
            ms *= (1000 * 60 * 60 * 24);
            break;
        }
        case TimeSpan::Month: {
            ms *= (u64)(1000 * 60 * 60);
            ms *= (24 * 31);
            break;
        }
        case TimeSpan::Year: {
            ms *= (u64)(1000 * 60 * 60);
            ms *= (24 * 365);
            break;
        }
    }
    sleep(ms);
}

void AcpiTimerDevice::sleep(u64 ms) {
    u64 clock   = 3579545 * ms / 1000;
    u64 counter = 0;
    u64 last    = m_reader(m_data);
    u64 current = 0;
    while (counter < clock) {
        current = m_reader(m_data);
        if (current < last) {
            counter += (m_is32bitMode ? 0x100000000ul : 0x1000000) + current - last;
        } else {
            counter += current - last;
        }
        last = current;
        asm("nop");
    }
}