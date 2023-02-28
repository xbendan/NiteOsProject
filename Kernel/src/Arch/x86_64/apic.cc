#include <Arch/x86_64/apic.h>
#include <Arch/x86_64/acpi.h>
#include <Arch/x86_64/irq.h>
#include <Arch/x86_64/pic.h>
#include <Arch/x86_64/mmu.h>
#include <Arch/x86_64/cpu.h>
#include <drv/video.h>
#include <kern.h>

void SpuriousInterruptHandler(void *, registers_t * regs) { Video::WriteText("Spurious interrupted."); }

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

            virtualBase = Memory::ManagementUnit::GetIOMapping(base);
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
        uintptr_t localPhysApicBase;
        volatile uint32_t *localApicBase;

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

        void WriteData(uint32_t reg, uint32_t data)
        {
            *((volatile uint32_t*)(localApicBase + reg)) = data;
        }

        uint32_t ReadData(uint32_t reg)
        {
            return *((volatile uint32_t*)(localApicBase + reg));
        }

        void StartTimer()
        {

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
            // WriteBase(ReadBase() | (1UL << 11));
            WriteData(LOCAL_APIC_SIVR, ReadData(LOCAL_APIC_SIVR) | 0x1ff);
        }

        void Initialize()
        {
            if (!localPhysApicBase) {
                CallPanic("Local APIC base not found.");
            }

            localApicBase = (uint32_t *) Memory::ManagementUnit::GetIOMapping(localPhysApicBase);
            RegisterInterruptHandler(0xff, SpuriousInterruptHandler);

            Enable();
        }

        void EndOfInterrupt() { WriteData(LOCAL_APIC_EOI, 0); }
    } // namespace Local

    namespace Timer
    {
        uint32_t EstimateBusSpeed() {
            Local::WriteData(LOCAL_APIC_LVT_TIMER, 0x10000);
            Local::WriteData(LOCAL_APIC_TIMER_DIVIDE, 0x3);
            Local::WriteData(LOCAL_APIC_LVT_TIMER, 0x0);

            uint32_t val = 0xffffffff;
            Local::WriteData(LOCAL_APIC_TIMER_INITIAL_COUNT, val);
            
        }
    } // namespace Timer
    
    
    void Initialize()
    {
        //InterruptsRetainer();

        WriteMsr(ModelSpecificRegisters::MSR_APIC, 
                (ReadMsr(ModelSpecificRegisters::MSR_APIC) | 0x800) & ~(LOCAL_APIC_ENABLE));

        PIC::Disable();
        Local::Initialize();
        IO::Initialize();
    }
} // namespace APIC