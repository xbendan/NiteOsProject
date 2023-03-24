#include <Drivers/Generic/ACPI.h>
#include <Drivers/HPET.h>
#include <Mem/MMIO.h>
#include <System.h>

#include <Utils/LinkedList.h>
#include <Utils/ArrayList.h>
#include <Utils/Table.h>
#include <Utils/Range.h>

#include <Arch/x86_64/mmu.h>
#include <macros>

namespace ACPI
{
    SizedArrayList<uint8_t, 256> g_Processors;
    SizedArrayList<MADTInterruptOverride *, 256> g_InterruptOverrides;

    RootSystemDescriptionPointer *rsdp;
    RootSystemDescriptionTable *rsdt;
    ExtendedSystemDescriptionTable *xsdt;
    KeyValueTable<uint64_t, uint64_t> xsdtEntryTable;
    
    MADT *g_MADT;
    FADT *g_FADT;
    HPET *g_HPET;
    MCFG *g_MCFG;

    ACPITable *FindTableByName(const char *str, int index)
    {
        if (memcmp("DSDT", str, 4) == 0)
            return (ACPITable *) Memory::GetIOMapping(g_FADT->DSDT);

        if (!rsdp)
        {
            return nullptr;
        }

        uint64_t entries = rsdp->Revision ?
            (xsdt->Length - sizeof(ACPITable)) / sizeof(uint64_t) :
            (rsdt->Length - sizeof(ACPITable)) / sizeof(uint32_t);

        int _index = 0;
        for (int i = 0; i < entries; i++)
        {
            uintptr_t entry = rsdp->Revision ?
                xsdt->Pointers[i] :
                rsdt->Pointers[i];
            ACPITable *header = reinterpret_cast<ACPITable *>(Memory::GetIOMapping(entry));
            if (memcmp(header->Signature, str, 4) == 0 && _index++ == index) {
                return header;
            }
        }

        return nullptr;
    }

    void Initialize()
    {
        auto _lambda_FindRsdp = [](void) -> RootSystemDescriptionPointer *
        {
            const char *signature = "RSD PTR ";
            ValueRange ranges[3] = {
                { 0x0, 0x7BFF + 1 },
                { 0x80000, 0x9FFFF + 1 },
                { 0xE0000, 0xFFFFF + 1 }
            };
            for (size_t i = 0; i < 3; i++)
            {
                for (uint64_t addr = ranges[i].Start; addr < ranges[i].End; addr += 16)
                {
                    if (memcmp((void *) Memory::GetIOMapping(addr), signature, 8) == 0)
                    {
                        return reinterpret_cast<RootSystemDescriptionPointer *>(addr);
                    }
                }
            }
            return nullptr;
        };

        /*
         * Detect ACPI Root System Description Pointer (RSDP)
         * We cannot initialize ACPI without it.
         */
        if ((rsdp = _lambda_FindRsdp()) == nullptr)
            System::Panic("Cannot find ACPI Root System Description Pointer.");

        /*
         * Handle ACPI Revision
         * When Revision 
         *  ->  2           = ACPI Version 2.0 ~ 6.3
         *                       ACPI 2.0+ contains an extended structure so RSDP needs
         *                       to be "reinterpret_cast" as XSDP
         *  ->  0           = ACPI Version 1.0
         * If non of them above matches the revision, call kernel panic
         * because there shouldn't be any exceptions.
         */
        switch (rsdp->Revision)
        {
            case 0: {
                rsdt = reinterpret_cast<RootSystemDescriptionTable *>(Memory::GetIOMapping(rsdp->RsdtAddress));
                break;
            }
            case 2: {
                ExtendedSystemDescriptionPointer *xsdp = static_cast<ExtendedSystemDescriptionPointer *>(rsdp);

                rsdt = reinterpret_cast<RootSystemDescriptionTable *>(Memory::GetIOMapping(rsdp->RsdtAddress));
                xsdt = reinterpret_cast<ExtendedSystemDescriptionTable *>(Memory::GetIOMapping(xsdp->XsdtAddress));

                break;
            }
            default:
                System::Panic("Unknown ACPI Revision: %u", rsdp->Revision);
                break;
        }
        
        g_MADT = static_cast<MADT *>(FindTableByName("APIC", 0));
        g_FADT = static_cast<FADT *>(FindTableByName("FACP", 0));
        g_HPET = static_cast<HPET *>(FindTableByName("HPET", 0));
        g_MCFG = static_cast<MCFG *>(FindTableByName("MCFG", 0));

        g_Timers[TimerACPI] = new ACPITimer();
        g_Timers[TimerHPET] = new HPETTimer();
    }

    uint32_t GetRemapIRQ(uint32_t irq)
    {
        for (int i = 0; i < g_InterruptOverrides.Length(); i++) {
            MADTInterruptOverride *iso = g_InterruptOverrides[i];
            if (iso->irqSource == irq) {
                return iso->gSi;
            }
        }
        return -1;
    }
} // namespace ACPI
