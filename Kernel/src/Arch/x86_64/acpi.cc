#include <Arch/x86_64/ACPI.h>
#include <Arch/x86_64/APIC.h>
#include <Arch/x86_64/MMU.h>
#include <Arch/x86_64/IRQ.h>
#include <mm/mem.h>
#include <Kernel>

namespace ACPI
{
    const char *__acpi_Signature = "RSD PTR ";
    uint8_t g_Processors[256];
    int g_ProcessorCount = 1;
    acpi_rsdp_t *acpiDesc;
    acpi_rsdt_t *acpiRsdtHeader;
    acpi_xsdt_t *acpiXsdtHeader;
    acpi_fadt_t *acpiFadt;
    MadtIso *g_Isos[256];
    uint8_t g_IsoAmount;
    pci_mcfg_t *pciMcfg;

    char acpiOemId[7];

    uintptr_t __acpi_GetEntry(uint32_t index)
    {
        if (acpiDesc->revision == 2)
            return acpiXsdtHeader->pointers[index];
        else
            return acpiRsdtHeader->pointers[index];
    }

    void *FindTable(const char *str, int index)
    {
        int entries = 0;
        if (!acpiDesc)
        {
            return NULL;
        }

        if (acpiDesc->revision == 2)
            entries = (acpiRsdtHeader->table.length - sizeof(acpi_header_t)) / sizeof(uint64_t);
        else
            entries = (acpiRsdtHeader->table.length - sizeof(acpi_header_t)) / sizeof(uint32_t);

        int _index = 0;
        if (memcmp("DSDT", str, 4) == 0)
            return (void *) Memory::GetIOMapping(acpiFadt->dsdt);
            
        for (int i = 0; i < entries; i++)
        {
            acpi_header_t *header = (acpi_header_t *)(Memory::GetIOMapping(__acpi_GetEntry(i)));
            if (memcmp(header->signature, str, 4) == 0 && _index++ == index)
                return header;
        }

        return NULL;
    }

    void Initialize()
    {
        for (uintptr_t addr = 0; addr <= 0x7BFF; addr += 16)
        {
            if (memcmp((void *)Memory::GetIOMapping(addr), __acpi_Signature, 8) == 0)
            {
                acpiDesc = (acpi_rsdp_t *)(Memory::GetIOMapping(addr));
                goto INIT_ACPI_FOUND;
            }
        }

        for (uintptr_t addr = 0x80000; addr <= 0x9FFFF; addr += 16)
        {
            if (memcmp((void *)Memory::GetIOMapping(addr), __acpi_Signature, 8) == 0)
            {
                acpiDesc = (acpi_rsdp_t *)(Memory::GetIOMapping(addr));
                goto INIT_ACPI_FOUND;
            }
        }

        for (uintptr_t addr = 0xE0000; addr <= 0xFFFFF; addr += 16)
        {
            if (memcmp((void *)Memory::GetIOMapping(addr), __acpi_Signature, 8) == 0)
            {
                acpiDesc = (acpi_rsdp_t *)(Memory::GetIOMapping(addr));
                goto INIT_ACPI_FOUND;
            }
        }

        CallPanic("[ERR] No ACPI table found.");

    INIT_ACPI_FOUND:

        if(acpiDesc->revision == 2)
        {
            acpiRsdtHeader = ((acpi_rsdt_t *) Memory::GetIOMapping(((acpi_xsdp_t *) acpiDesc)->xsdtPtr));
            acpiXsdtHeader = ((acpi_xsdt_t *) Memory::GetIOMapping(((acpi_xsdp_t *) acpiDesc)->xsdtPtr));
        }
        else
            acpiRsdtHeader = ((acpi_rsdt_t *) Memory::GetIOMapping(acpiDesc->rsdtPtr));

        memcpy(acpiOemId, acpiRsdtHeader->table.oemId, 6);
        acpiOemId[6] = 0;

        acpiFadt = (acpi_fadt_t *)(FindTable("FACP", 0));

        DisableInterrupts();
        acpi_madt_t *madt = (acpi_madt_t *)(FindTable("APIC", 0));
        uintptr_t madtEnd = ((uintptr_t)madt) + madt->header.length;
        uintptr_t madtEntry = ((uintptr_t)madt) + sizeof(acpi_madt_t);

        while (madtEntry < madtEnd)
        {
            madt_entry_t *entry = (madt_entry_t *)(madtEntry);
            switch (entry->type)
            {
            case 0:
            {
                madt_local_t *apicLocal = (madt_local_t *)(entry);
                if (apicLocal->flags & 0x3)
                {
                    if (apicLocal->apicId == 0)
                        break;

                    g_Processors[g_ProcessorCount++] = apicLocal->apicId;
                }
                break;
            }
            case 1:
            {
                madt_io_t *apicIo = (madt_io_t *)(entry);

                if (!apicIo->gSib)
                    APIC::IO::SetBase(apicIo->address);
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
                break;
            }
            case 5:
            {
                break;
            }
            default:
                break;
            }

            madtEntry += entry->length;
        }

        pciMcfg = (pci_mcfg_t *)(FindTable("MCFG", 0));

        EnableInterrupts();
    }
}