#include <Drivers/APIC.h>
#include <Mem/MMIO.h>
#include <Mem/KMemAlloc.h>
#include <Mem/SlabAllocator.h>
#include <System.h>
#include <Timer.h>

#include <Arch/x86_64/CPU.h>
#include <Arch/x86_64/IDT.h>
#include <Arch/x86_64/Interrupts.h>
#include <Arch/x86_64/pic.h>

void SpuriousInterruptHandler(void *, registers_t * regs) { System::Out("Spurious interrupted."); }

namespace APIC::Local
{
    uint64_t BasePhys;
    uint64_t BaseVirtIO;

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
        asm("rdmsr"
            : "=a"(low), "=d"(high)
            : "c"(0x1B));

        return ((high & 0x0f) << 32) | (low & 0xfffff000);
    }

    void WriteData(
        uint32_t reg,
        uint32_t data)
    {
        *((volatile uint32_t *)(BaseVirtIO + reg)) = data;
    }

    uint32_t ReadData(
        uint32_t reg)
    {
        return *((volatile uint32_t *)(BaseVirtIO + reg));
    }

    void SendIPI(uint8_t apicId, uint32_t vector)
    {
        WriteData(LOCAL_APIC_ICR_HIGH, ((uint32_t)apicId) << 24);
        WriteData(LOCAL_APIC_ICR_LOW, vector);

        while ((ReadData((uint32_t)LOCAL_APIC_ICR_LOW) & LOCAL_APIC_ICR_PENDING) != 0)
            ;
    }

    void SendIPI(uint8_t apicId, uint32_t dsh, uint32_t type, uint8_t vector)
    {
        SendIPI(apicId, dsh | type | ICR_VECTOR(vector));
    }

    void Enable()
    {
        WriteData(LOCAL_APIC_SIVR, 0x1FF);
    }

    void Initialize()
    {
        // if (!BasePhys)
        // {
        //     System::Panic("Local APIC base not found.");
        // }

        BasePhys = ReadBase();
        BaseVirtIO = Memory::GetIOMapping(BasePhys);

        // WriteMsr(ModelSpecificRegisters::MSR_APIC,
        //     (ReadMsr(ModelSpecificRegisters::MSR_APIC) | 0x800) & ~(LOCAL_APIC_ENABLE));

        System::Out("Local APIC Base: %x", BaseVirtIO);

        Enable();
        PIC::Disable();

        RegisterIRQ(0xFF, SpuriousInterruptHandler);
    }

    void EndOfInterrupt() { WriteData(LOCAL_APIC_EOI, 0); }
} // namespace APIC
