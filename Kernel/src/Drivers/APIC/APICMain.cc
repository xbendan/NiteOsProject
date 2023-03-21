#include <Drivers/APIC.h>
#include <Drivers/ACPI.h>
#include <System.h>

#include <Arch/x86_64/cpu.h>

namespace APIC
{
    void Initialize()
    {
        using namespace ACPI;
        
        if (g_MADT == nullptr)
        {
            System::Panic("Cannot find Multi ACPI Description Table (MADT).");
            return;
        }
        System::Out("Initializing APIC, Base=%x", Local::BasePhys = g_MADT->Address);
        
        uint64_t offset = (uint64_t) &g_MADT->Entries;
        uint64_t end = reinterpret_cast<uint64_t>(g_MADT) + g_MADT->Length;
        while (offset < end)
        {
            MADTEntry *entry = reinterpret_cast<MADTEntry *>(offset);
            switch (entry->Type)
            {
                case 0: {
                    MADTLocalApic *apicLocal = static_cast<MADTLocalApic *>(entry);
                    if (apicLocal->Flags & 0x3 && apicLocal->ApicId)
                    {
                        g_Processors.Add(apicLocal->ApicId);
                        System::Out("[ACPI] Processor found. APIC ID=%u, Flags=%u", apicLocal->ApicId, apicLocal->Flags);
                    }
                    break;
                }
                case 1: {
                    MADTIoApic *apicIO = static_cast<MADTIoApic *>(entry);
                    if (!apicIO->GSIB)
                    {
                        System::Out("[ACPI] I/O APIC address set to %x", apicIO->Address);
                        APIC::IO::SetBase(apicIO->Address);
                    }
                    break;
                }
                case 2: {
                    g_InterruptOverrides.Add(static_cast<MADTInterruptOverride *>(entry));
                    System::Out("[ACPI] Interrupt Override: %x", (uintptr_t) entry);
                    break;
                }
                case 3: {
                    MADTNonmaskableInterrupt *nmi = static_cast<MADTNonmaskableInterrupt *>(entry);
                    System::Out("[ACPI] Non-maskable interrupt, lInt=%x", nmi->LINT);
                    break;
                }
                case 5: /* Local APIC address override */ {
                    System::Out("[ACPI] APIC Address Override");
                    break;
                }
                case 9: /* Processor Local x2APIC */ {
                    System::Out("[ACPI] x2APIC detected.");
                    break;
                }
                default: break;
            }

            offset += entry->Length;
        }

        WriteMsr(ModelSpecificRegisters::MSR_APIC, 
            (ReadMsr(ModelSpecificRegisters::MSR_APIC) | 0x800) & ~(LOCAL_APIC_ENABLE));

        Local::Initialize();
        IO::Initialize();
    }
} // namespace APIC
