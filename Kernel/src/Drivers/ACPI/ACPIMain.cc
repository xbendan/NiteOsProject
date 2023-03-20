#include <Drivers/ACPI.h>
#include <Mem/MMIO.h>
#include <system.h>

#include <macros>

namespace ACPI
{
    RootSystemDescriptionPointer *rsdp;

    RootSystemDescriptionTable *rsdt;
    ExtendedSystemDescriptionTable *xsdt;

    char g_OemId[6];

    ACPITable *FindTableByName(const char *str, int index)
    {
        if (memcmp("DSDT", str, 4) == 0)
            return (void *) Memory::GetIOMapping(acpiFadt->DSDT);

        if (!rsdp)
        {
            return nullptr;
        }

        uint64_t length = rsdt->Length - sizeof(ACPITable);
        uint64_t entries = rsdp->Revision ?
            length / sizeof(uint64_t) :
            length / sizeof(uint32_t);

        int _index = 0;
        for (int i = 0; i < entries; i++)
        {
            uintptr_t entry = rsdp->Revision ?
                xsdt->Pointers[i] :
                rsdt->Pointers[i];
            ACPITable *header = reinterpret_cast<ACPITable *>(Memory::GetIOMapping(entry));
            if (memcmp(header->signature, str, 4) == 0 && ++_index == index) {
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
            ValueRange ranges[3] = [
                { .Start = 0x0, .End = 0x7BFF + 1 },
                { .Start = 0x80000, .End = 0x9FFFF + 1 },
                { .Start = 0xE0000, .End = 0xFFFFF + 1 }
            ];
            for (size_t i = 0; i < 3; i++)
            {
                for (uint64_t addr = ranges[i].Start; addr < ranges[i].End; addr += 16)
                {
                    if (memcmp(Memory::GetIOMapping(addr), signature, 8) == 0)
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
        if ((rsdp = _lambda_FindRsdp) == nullptr)
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
                uint64_t xsdtAddress = Memory::GetIOMapping(xsdp->XsdtAddress);

                rsdt = reinterpret_cast<RootSystemDescriptionTable *>(xsdtAddress);
                xsdt = reinterpret_cast<ExtendedSystemDescriptionTable *>(xsdtAddress);
                break;
            }
            default:
                System::Panic("Unknown ACPI Revision: %u", rsdp->Revision);
                break;
        }
    
        memcpy(g_OemID, rsdt->OemID, 6);
    }
} // namespace ACPI
