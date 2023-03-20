#include <Arch/x86_64/apic.h>
#include <Arch/x86_64/acpi.h>
#include <Arch/x86_64/irq.h>
#include <Arch/x86_64/pic.h>
#include <Arch/x86_64/mmu.h>
#include <Arch/x86_64/cpu.h>
#include <system.h>
#include <kern.h>

void SpuriousInterruptHandler(void *, registers_t * regs) { System::Out("Spurious interrupted."); }

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

        void Initialize() {
            if(!base)
            {
                System::Panic("No I/O APIC base address found.");
                return;
            }

            virtualBase = Memory::ManagementUnit::GetIOMapping(base);
            registerSelect = (uint32_t *)(virtualBase + IO_APIC_REGSEL);
            ioWindow = (uint32_t *)(virtualBase + IO_APIC_WIN);

            interrupts = ReadData32(IO_APIC_REGISTER_VER) >> 16;
            apicId = ReadData32(IO_APIC_REGISTER_ID) >> 24;

            for (int i = 0; i < ACPI::g_IsoAmount; i++)
            {
                MadtIso *iso = ACPI::g_Isos[i];
                // Redirect(iso->gSi, iso->irqSource + 0x20, 0);
                System::Out("Interrupt Override found, source=%u, interrupt=%u", iso->irqSource, iso->gSi);
            }
        }

        void Redirect(
            uint8_t         irq, 
            uint8_t         vector, 
            uint32_t        delivery
        ) {
            WriteData64(IO_APIC_RED_TABLE_ENT(irq), delivery | vector);
        }

        void SetBase(uintptr_t newBase)
        {
            base = newBase;
        }
        
        void MapLegacyIRQ(uint8_t irq) {
            uint8_t index = ACPI::GetRemapIRQ(irq);
            WriteData64(IO_APIC_RED_TABLE_START + (index * 2), irq);
            System::Out("Map legacy IRQ index=%u, irq=%u", index, irq);
        }
    } // namespace IO

    namespace Local
    {
        uintptr_t localPhysApicBase;
        uintptr_t localApicBase;

        void WriteBase(uint64_t val)
        {
            uint64_t low = val & 0xFFFFFFFF | 0x800;
            uint64_t high = val >> 32;
            asm("wrmsr" ::"a"(low), "d"(high), "c"(0x1B));
        }

        uint64_t ReadBase()
        {
            uint64_t low;
            uint64_t high;
            asm("rdmsr" : "=a"(low), "=d"(high) : "c"(0x1B));

            return ((high & 0x0f) << 32) | (low & 0xfffff000);
        }

        void WriteData(
            uint32_t        reg, 
            uint32_t        data
        ) {
            *((volatile uint32_t *) (localApicBase + reg)) = data;
            //*((volatile uint32_t *)(localApicBase + reg)) = data;
        }

        uint32_t ReadData(
            uint32_t        reg
        ) {
            return *((volatile uint32_t *)(localApicBase + reg));
            //return *((volatile uint32_t *)(localApicBase + reg));
        }

        void SendIPI(uint8_t apicId, uint32_t vector) {
            WriteData(LOCAL_APIC_ICR_HIGH, ((uint32_t) apicId) << 24);
            WriteData(LOCAL_APIC_ICR_LOW, vector);

            while ((ReadData((uint32_t) LOCAL_APIC_ICR_LOW) & LOCAL_APIC_ICR_PENDING) != 0);
        }

        void SendIPI(uint8_t apicId, uint32_t dsh, uint32_t type, uint8_t vector) {
            SendIPI(apicId, dsh | type | ICR_VECTOR(vector));
        }
        
        void Enable() {
            WriteData(LOCAL_APIC_SIVR, 0x100);
        }

        void Initialize()
        {
            if (!localPhysApicBase) {
                System::Panic("Local APIC base not found.");
            }

            localApicBase = Memory::ManagementUnit::GetIOMapping(localPhysApicBase);

            WriteMsr(ModelSpecificRegisters::MSR_APIC, 
                (ReadMsr(ModelSpecificRegisters::MSR_APIC) | 0x800) & ~(LOCAL_APIC_ENABLE));

            System::Out("%X", ReadBase());

            Enable();
            PIC::Disable();

            Timer::Initialize(1000, 0x20);

            RegisterInterruptHandler(0xff, SpuriousInterruptHandler);
        }

        void EndOfInterrupt() { WriteData(LOCAL_APIC_EOI, 0); }
    } // namespace Local

    namespace Timer
    {
        volatile uint32_t Ticks() {
            return Local::ReadData(LOCAL_APIC_TIMER_CURRENT_COUNT);
        }

        uint32_t EstimateBusSpeed() {
            Local::WriteData(LOCAL_APIC_LVT_TIMER, 0x10000);
            Local::WriteData(LOCAL_APIC_TIMER_DIVIDE, 0x3);
            Local::WriteData(LOCAL_APIC_LVT_TIMER, 0x0);

            uint32_t val = 0xffffffff;
            Local::WriteData(LOCAL_APIC_TIMER_INITIAL_COUNT, val);

            ACPI::Timer::Sleep(100000);

            Local::WriteData(LOCAL_APIC_LVT_TIMER, 0x10000);
            uint64_t busFreq = val - Local::ReadData(LOCAL_APIC_TIMER_CURRENT_COUNT);
            
            return busFreq;
        }

        void Initialize(uint64_t hertz, uint32_t irq) {
            Local::WriteData(LOCAL_APIC_LVT_TIMER, 0x00020000 | irq);
            Local::WriteData(LOCAL_APIC_TIMER_DIVIDE, 0x3);
            Local::WriteData(LOCAL_APIC_TIMER_INITIAL_COUNT, EstimateBusSpeed() / 16 / hertz);

            IO::MapLegacyIRQ(0x20);

            for (int i = 0; i < 4; i++) {
                System::Out("%u", Local::ReadData(0X390));
                ACPI::Timer::Sleep(100000);
            }
        }
    } // namespace Timer
    
    
    void Initialize()
    {
        //InterruptsRetainer();

        

        // WriteMsr(ModelSpecificRegisters::MSR_APIC, 
        //         (ReadMsr(ModelSpecificRegisters::MSR_APIC) | 0x800) & ~(LOCAL_APIC_ENABLE));

        Local::Initialize();
        IO::Initialize();
    }
} // namespace APIC