#include <Arch/x86_64/APIC.h>
#include <Arch/x86_64/ACPI.h>
#include <Arch/x86_64/IRQ.h>
#include <Arch/x86_64/PIC.h>
#include <Arch/x86_64/MMU.h>
#include <Kernel>

namespace APIC
{
    namespace IO
    {
        uintptr_t base = 0;
        uintptr_t virtualBase;
        volatile uint32_t* registerSelect;
        volatile uint32_t* ioWindow;

        uint32_t interrupts;
        uint32_t apicId;

        void WriteData32(uint32_t reg, uint32_t data)
        {
            *registerSelect = reg;
            *ioWindow = data;
        }

        uint32_t ReadData32(uint32_t reg)
        {
            *registerSelect = reg;
            return *ioWindow;
        }

        void WriteData64(uint32_t reg, uint64_t data)
        {
            uint32_t low = data & 0xFFFFFFFF;
            uint32_t high = data >> 32;

            WriteData32(reg, low);
            WriteData32(reg + 1, high);
        }

        uint64_t ReadData64(uint32_t reg)
        {
            uint32_t low = ReadData32(reg), high = ReadData32(reg + 1);
            return low | ((uint64_t)(high) << 32);
        }

        void Initialize()
        {
            if(!base)
            {
                CallPanic("No I/O APIC base address.");
                return;
            }

            virtualBase = Memory::GetIOMapping(base);
            registerSelect = (uint32_t *)(virtualBase + IO_APIC_REGSEL);
            ioWindow = (uint32_t *)(virtualBase + IO_APIC_WIN);

            interrupts = ReadData32(IO_APIC_REGISTER_VER) >> 16;
            apicId = ReadData32(IO_APIC_REGISTER_ID) >> 24;

            for (int i = 0; i < ACPI::g_IsoAmount; i++)
            {
                MadtIso *iso = ACPI::g_Isos[i];
                Redirect(iso->gSi, iso->irqSource + 0x20, 0);
            }
        }

        void Redirect(uint8_t irq, uint8_t vector, uint32_t delivery)
        {
            WriteData64(IO_APIC_RED_TABLE_ENT(irq), delivery | vector);
        }

        void SetBase(uintptr_t newBase)
        {
            base = newBase;
        }
        
        void MapLegacyIRQ(uint8_t irq);
    } // namespace IO
    namespace Local
    {
        volatile uint32_t *basePtr;

        void WriteBase(uint64_t val)
        {
            uint64_t low = val & 0xFFFFFFFF;
            uint64_t high = val >> 32;
            asm("wrmsr" ::"a"(low), "d"(high), "c"(0x1B));
        }

        uint64_t ReadBase()
        {
            uint64_t low;
            uint64_t high;
            asm("rdmsr" : "=a"(low), "=d"(high) : "c"(0x1B));
        }

        void WriteData(uint32_t reg, uint32_t data)
        {
            *((volatile uint32_t*)(basePtr + reg)) = data;
        }

        uint32_t ReadData(uint32_t reg)
        {
            return *((volatile uint32_t*)(basePtr + reg));
        }

        void StartTimer()
        {

        }

        void SendIPI(uint8_t apicId, uint32_t dsh, uint32_t type, uint8_t vector)
        {
            uint32_t high = ((uint32_t) apicId) << 24;
            uint32_t low = dsh | type | ICR_VECTOR(vector);

            WriteData(LOCAL_APIC_ICR_HIGH, high);
            WriteData(LOCAL_APIC_ICR_LOW, low);
        }

        void Initialize()
        {
            basePtr = (uint32_t *) Memory::GetIOMapping(ReadBase());
            WriteBase(ReadBase() | (1UL << 11));
            WriteData(
                LOCAL_APIC_SIVR,
                ReadData(LOCAL_APIC_SIVR) | 0x100
            );
        }
    } // namespace Local
    
    void Initialize()
    {
        DisableInterrupts();

        PIC::Disable();
        Local::Initialize();
        IO::Initialize();

        EnableInterrupts();
    }
} // namespace APIC