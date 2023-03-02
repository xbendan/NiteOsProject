#include <Arch/x86_64/acpi.h>
#include <Arch/x86_64/apic.h>
#include <Arch/x86_64/mmu.h>
#include <Arch/x86_64/irq.h>
#include <Arch/x86_64/ports.h>
#include <drv/video.h>
#include <mm/mem.h>
#include <kern.h>
#include <system.h>

namespace ACPI
{
    const char *__acpi_Signature = "RSD PTR ";
    uint8_t g_Processors[256];
    int g_ProcessorCount = 1;
    acpi_rsdp_t *acpiDesc;
    acpi_rsdt_t *acpiRsdtHeader;
    acpi_xsdt_t *acpiXsdtHeader;
    MadtIso *g_Isos[256];
    uint8_t g_IsoAmount;

    acpi_fadt_t *acpiFadt;
    acpi_hpet_t *acpiHpet;
    pci_mcfg_t *pciMcfg;

    char acpiOemId[7];

    uint32_t *acpiTimerBlock;

    void *FindTable(const char *str, int index)
    {
        int entries = 0;
        if (!acpiDesc)
        {
            return nullptr;
        }

        if (acpiDesc->revision == 2)
            entries = (acpiRsdtHeader->table.length - sizeof(acpi_header_t)) / sizeof(uint64_t);
        else
            entries = (acpiRsdtHeader->table.length - sizeof(acpi_header_t)) / sizeof(uint32_t);

        auto getEntry = [](unsigned index) -> uintptr_t {
            if (acpiDesc->revision == 2)
                return acpiXsdtHeader->pointers[index];
            else
                return acpiRsdtHeader->pointers[index];
        };

        int _index = 0;

        if (memcmp("DSDT", str, 4) == 0)
            return (void *) Memory::ManagementUnit::GetIOMapping(acpiFadt->dsdt);

        for (int i = 0; i < entries; i++)
        {
            uintptr_t entry = getEntry(i);
            acpi_header_t *header = (acpi_header_t *)(Memory::ManagementUnit::GetIOMapping(entry));
            if (memcmp(header->signature, str, 4) == 0 && _index++ == index) {
                return header;
            }
        }

        return nullptr;
    }

    void Initialize()
    {
        for (uintptr_t addr = 0; addr <= 0x7BFF; addr += 16)
        {
            if (memcmp((void *)Memory::ManagementUnit::GetIOMapping(addr), __acpi_Signature, 8) == 0)
            {
                acpiDesc = (acpi_rsdp_t *)(Memory::ManagementUnit::GetIOMapping(addr));
                goto INIT_ACPI_FOUND;
            }
        }

        for (uintptr_t addr = 0x80000; addr <= 0x9FFFF; addr += 16)
        {
            if (memcmp((void *)Memory::ManagementUnit::GetIOMapping(addr), __acpi_Signature, 8) == 0)
            {
                acpiDesc = (acpi_rsdp_t *)(Memory::ManagementUnit::GetIOMapping(addr));
                goto INIT_ACPI_FOUND;
            }
        }

        for (uintptr_t addr = 0xE0000; addr <= 0xFFFFF; addr += 16)
        {
            if (memcmp((void *)Memory::ManagementUnit::GetIOMapping(addr), __acpi_Signature, 8) == 0)
            {
                acpiDesc = (acpi_rsdp_t *)(Memory::ManagementUnit::GetIOMapping(addr));
                goto INIT_ACPI_FOUND;
            }
        }

        CallPanic("[ERR] No ACPI table found.");

    INIT_ACPI_FOUND:

        if(acpiDesc->revision == 2)
        {
            acpiRsdtHeader = ((acpi_rsdt_t *) Memory::ManagementUnit::GetIOMapping(((acpi_xsdp_t *) acpiDesc)->xsdtPtr));
            acpiXsdtHeader = ((acpi_xsdt_t *) Memory::ManagementUnit::GetIOMapping(((acpi_xsdp_t *) acpiDesc)->xsdtPtr));
        }
        else
            acpiRsdtHeader = ((acpi_rsdt_t *) Memory::ManagementUnit::GetIOMapping(acpiDesc->rsdtPtr));

        memcpy(acpiOemId, acpiRsdtHeader->table.oemId, 6);
        acpiOemId[6] = 0;

        acpi_madt_t *madt = (acpi_madt_t *)(FindTable("APIC", 0));
        APIC::Local::localPhysApicBase = madt->address;
        
        uintptr_t madtEntriesEnd = ((uintptr_t)madt) + madt->length;
        uintptr_t madtEntries = (uintptr_t) &madt->entries;

        while (madtEntries < madtEntriesEnd)
        {
            madt_entry_t *entry = (madt_entry_t *)(madtEntries);
            switch (entry->type)
            {
            case 0:
            {
                madt_local_t *apicLocal = (madt_local_t *)(entry);
                if (apicLocal->flags & 0x3)
                {
                    if (apicLocal->apicId != 0) {
                        g_Processors[g_ProcessorCount++] = apicLocal->apicId;
                        System::Out("[ACPI] Processor found. APIC ID=%u, Flags=%u", apicLocal->apicId, apicLocal->flags);
                    }
                }
                break;
            }
            case 1:
            {
                madt_io_t *apicIo = (madt_io_t *)(entry);

                if (!apicIo->gSib) {
                    System::Out("[ACPI] I/O APIC address set to %x", apicIo->address);
                    APIC::IO::SetBase(apicIo->address);
                }
                
                break;
            }
            case 2:
            {
                madt_iso_t *iso = (madt_iso_t *)(entry);
                g_Isos[g_IsoAmount++] = iso;
                break;
            }
            case 3:
            {
                break;
            }
            case 4:
            {
                MadtNmi *nmi = reinterpret_cast<MadtNmi *>(entry);
                System::Out("[ACPI] Non-maskable interrupt, lInt=%x", nmi->lInt);
                break;
            }
            case 5: /* Local APIC address override */
            {
                System::Out("[ACPI] APIC Address Override");
                break;
            }
            case 9: /* Processor Local x2APIC */ {
                System::Out("[ACPI] x2APIC detected.");

                break;
            }
            default:
                break;
            }

            madtEntries += entry->length;
        }

        pciMcfg = (pci_mcfg_t *)(FindTable("MCFG", 0));
        acpiFadt = (acpi_fadt_t *)(FindTable("FACP", 0));
        acpiHpet = static_cast<acpi_hpet_t *>(FindTable("HPET", 0));

        Timer::Initialize();
    }

    uint32_t GetRemapIRQ(uint32_t irq) {
        for (int i = 0; i < g_IsoAmount; i++) {
            madt_iso_t *iso = g_Isos[i];
            if (iso->irqSource == irq) {
                return iso->gSi;
            }
        }
        return irq;
    }

    namespace Timer
    {
        uint16_t timerTicks;

        void Initialize() {
            acpi_gas_t *xpm_timer = &acpiFadt->x_pmt_timer_block;
            uint64_t _acpiTimerBlock = 
                    // xpm_timer->address ?
                    // xpm_timer->address :
                    acpiFadt->pmt_timer_block;
            
            switch (acpiFadt->x_pmt_timer_block.addressSpace) {
                case 0: {
                    // uint64_t pmtVirt = Memory::ManagementUnit::KernelAllocate4KPages(1);
                    // uint64_t pmtPhys = ALIGN_DOWN(_acpiTimerBlock, ARCH_PAGE_SIZE);
                    // uint16_t offset = _acpiTimerBlock - pmtPhys;
                    
                    // Memory::ManagementUnit::KernelMapVirtualMemory4K(pmtPhys, pmtVirt, 1);
                    // timerTicks = reinterpret_cast<uint32_t *>(pmtVirt + offset);
                    timerTicks = acpiFadt->pmt_timer_block;
                    break;
                }
                case 1:
                    break;
                default:
                    System::Out("Not implemented %u, Reset to default", acpiFadt->x_pmt_timer_block.addressSpace);
                    timerTicks = acpiFadt->pmt_timer_block;
                    //CallPanic("Not implemented");
                    break;
            }
        }

        void Sleep(uint64_t microsecs) {
            if (acpiFadt->pmt_timer_length != 4) {
                return;
            }

            uint64_t clock = 3579545 * microsecs / 1000000;
            uint64_t counter = 0;
            uint64_t last = Ports::ReadDword32(timerTicks);
            uint64_t current = 0;

            while (counter < clock) {
                current = Ports::ReadDword32(timerTicks);
                if (current < last) {
                    bool isAcpiTimer32Bit = (acpiFadt->flags >> 8) & 0x1;
                    counter += (isAcpiTimer32Bit ? 0x100000000ul : 0x1000000) + current - last;
                } else {
                    counter += current - last;
                }
                last = current;
                asm("nop");
            }
        } 
    } // namespace Timer
}