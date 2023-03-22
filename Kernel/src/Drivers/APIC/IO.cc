#include <Drivers/APIC.h>
#include <Drivers/ACPI.h>
#include <Mem/MMIO.h>
#include <System.h>

namespace APIC::IO
{
    uintptr_t BasePhys = 0;
    uintptr_t BaseVirtIO;
    volatile uint32_t *registerSelect;
    volatile uint32_t *ioWindow;

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
        if (!BasePhys)
        {
            System::Panic("No I/O APIC base address found.");
            return;
        }

        BaseVirtIO = Memory::GetIOMapping(BasePhys);
        registerSelect = (uint32_t *)(BaseVirtIO + IO_APIC_REGSEL);
        ioWindow = (uint32_t *)(BaseVirtIO + IO_APIC_WIN);

        interrupts = ReadData32(IO_APIC_REGISTER_VER) >> 16;
        apicId = ReadData32(IO_APIC_REGISTER_ID) >> 24;

        // for (int i = 0; i < ACPI::g_InterruptOverrides.Length(); i++)
        // {
        //     MADTInterruptOverride *iso = ACPI::g_InterruptOverrides[i];
        //     // Redirect(iso->gSi, iso->irqSource + 0x20, 0);
        //     System::Out("Interrupt Override found, source=%u, interrupt=%u", iso->irqSource, iso->gSi);
        // }
    }

    void Redirect(
        uint8_t irq,
        uint8_t vector,
        uint32_t delivery)
    {
        WriteData64(IO_APIC_RED_TABLE_ENT(irq), delivery | vector);
    }

    void SetBase(uintptr_t newBase)
    {
        BasePhys = newBase;
    }

    void MapLegacyIRQ(uint8_t irq)
    {
        uint8_t index = ACPI::GetRemapIRQ(irq);
        WriteData64(IO_APIC_RED_TABLE_START + (index * 2), irq);
        System::Out("Map legacy IRQ index=%u, irq=%u", index, irq);
    }

    void EnableInterrupt(uint8_t irq)
    {
        uint8_t index = 0;
        // ACPI::g_InterruptOverrides.ForEach([&](MADTInterruptOverride *&obj, int) -> void {
        //     if (obj->irqSource == (irq - 0x20))
        //     {
        //         WriteData64(IO_APIC_RED_TABLE_ENT(obj->gSi), irq);
        //         System::Out("Remap IRQ: Index=%u Data=%u", obj->gSi, irq);
        //         return;
        //     }
        // });
        // for (int i = 0; i < ACPI::g_InterruptOverrides.Length(); i++)
        // {
        //     MADTInterruptOverride* iso = ACPI::g_InterruptOverrides[i];
        //     if (iso->irqSource == (irq - 0x20))
        //     {
        //         WriteData64(IO_APIC_RED_TABLE_ENT(iso->gSi), irq);
        //         System::Out("Remap IRQ: Index=%u Data=%u", iso->gSi, irq);
        //         return;
        //     }
        // }
    }
} // namespace APIC::IO
