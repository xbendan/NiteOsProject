#include <arch-x86_64/hwcpus.h>
#include <arch-x86_64/hwtypes.h>
#include <drivers/acpi/device.h>
#include <drivers/apic/device.h>
#include <drivers/apic/spec.h>
#include <siberix/dvc/type-processor.h>
#include <siberix/main.h>

namespace Kern::Hal::Impls {
    using namespace Platform::X64;

    ApicDevice::ApicDevice()
      : SmpSvcHost()
      , Device("Advanced Programmable Interrupt Controller")
      , m_interfaces(new Std::LinkedList<ApicLocal>())
      , m_overrides(new Std::LinkedList<MadtIso*>())
    {
    }

    void ApicDevice::onLoad()
    {
        Opt<Device> opt = Main::findDevice("ACPI Management Device");
        if (opt.isEmpty()) {
            return;
        }
        AcpiMgmtDevice* acpi = static_cast<AcpiMgmtDevice*>(opt.get());
        Madt*           madt;
        if (!(madt = acpi->findTable<Madt>("APIC"))) {
            // TODO: log
            return;
        }
        uint64_t offset = (uint64_t) & (madt->_entries[0]);
        uint64_t end    = (uint64_t)madt + madt->_length;
        while (offset < end) {
            MadtEntry* entry = reinterpret_cast<MadtEntry*>(offset);
            switch (entry->_type) {
                case 0x00: /* Processor Local APIC */ {
                    MadtLocalApic* apicLocal =
                      static_cast<MadtLocalApic*>(entry);
                    if (apicLocal->_flags & 0x3) {
                        ProcessorDevice* processor = new ProcessorDevice(
                          apicLocal->_processorId,
                          new CPUImplX64(apicLocal->_apicId,
                                         apicLocal->_processorId));
                        Main::registerDevice(processor);

                        m_interfaces->add(
                          new ApicLocal(apicLocal->_apicId, this, processor));
                    }
                    break;
                }
                case 0x01: /* IO APIC */ {
                    MadtIoApic* apicIo = static_cast<MadtIoApic*>(entry);
                    if (!apicIo->_gSiB)
                        m_ioBasePhys = apicIo->_address;
                    break;
                }
                case 0x02: /* Interrupt Source Override */ {
                    m_overrides->add(static_cast<MadtIso*>(entry));
                    break;
                }
                case 0x03: /* Non-maskable Interrupt */ {
                    break;
                }
                case 0x04: /* Local APIC Non-maskable Interrupt */ {
                    break;
                }
                case 0x05: /* Local APIC Address Override */ {
                    break;
                }
                case 0x09: /* Processor Local x2APIC */ {
                    break;
                }
                case 0xa: /* Local x2APIC Non-maskable Interrupt */ {
                    break;
                }
            }
            offset += entry->_length;
        }

        if (!m_ioBasePhys) {
            // TODO: log
            return;
        }

        m_ioBaseVirt = Kern::Mem::copyAsIoAddress(m_ioBasePhys);
        m_ioRegSel   = (uint32_t*)(m_ioBaseVirt + IO_APIC_REGSEL);
        m_ioWindow   = (uint32_t*)(m_ioBaseVirt + IO_APIC_WIN);
        m_interrupts = ioRegRead32(IO_APIC_REGISTER_VER) >> 16;

        if (rdmsr(0x1b) != 0) {
        }

        m_flags |= DEVICE_FLAG_LOADED;
    }

    void ApicDevice::ioRegWrite32(uint32_t reg, uint32_t data)
    {
        *m_ioRegSel = reg;
        *m_ioWindow = data;
    }

    uint32_t ApicDevice::ioRegRead32(uint32_t reg)
    {
        *m_ioRegSel = reg;
        return *m_ioWindow;
    }

    void ApicDevice::ioRegWrite64(uint32_t reg, uint64_t data)
    {
        uint32_t low  = data & 0xFFFFFFFF;
        uint32_t high = data >> 32;

        ioRegWrite32(reg, low);
        ioRegWrite32(reg + 1, high);
    }

    uint64_t ApicDevice::ioRegRead64(uint32_t reg)
    {
        uint32_t low  = ioRegRead32(reg);
        uint32_t high = ioRegRead32(reg + 1);
        return low | ((uint64_t)(high) << 32);
    }

    void ApicDevice::ioRedTblWrite(uint32_t index, uint64_t data)
    {
        ioRegWrite64(IO_APIC_RED_TABLE_ENT(index), data);
    }

    uint64_t ApicDevice::ioRedTblRead(uint32_t index)
    {
        return ioRegRead64(IO_APIC_RED_TABLE_ENT(index));
    }

    void ApicDevice::localBaseWrite(uint64_t data)
    {
        wrmsr(0x1b, data);
    }

    uint64_t ApicDevice::localBaseRead()
    {
        return rdmsr(0x1b);
    }

    void ApicDevice::localRegWrite(uint32_t reg, uint32_t data)
    {
        *((volatile uint32_t*)(m_ioBaseVirt + reg)) = data;
    }

    uint32_t ApicDevice::localRegRead(uint32_t reg)
    {
        return *((volatile uint32_t*)(m_ioBaseVirt + reg));
    }
}