#include <Arch/x86_64/acpi.h>
#include <Arch/x86_64/hpet.h>
#include <Arch/x86_64/ports.h>
#include <libkern/objects.h>
#include <system.h>
namespace HPET
{
    uint64_t clock;
    uint64_t ticks;

    inline void Write(uint64_t reg, uint64_t val) {
        *((uint64_t *)(ACPI::acpiHpet->address + reg)) = val;
    }

    inline uint64_t Read(uint64_t reg) {
        return *((uint64_t *) ACPI::acpiHpet->address + reg);
    }

    void Initialize() {
        if (Objects::IsNull(ACPI::acpiHpet)) {
            return;
        }

        clock = Read(0) >> 32;
        Write(0x10, 0);
        Write(0xf0, 0);
        Write(0x10, 1);

        System::Out("HPET Initialized.");
    }

    void Wait(uint64_t ms) {
        ticks = 0;
        uint64_t futureTicks = ticks + (ms * 10000000000000) / clock;
        while (ticks < futureTicks) asm("nop");
    }
} // namespace HPET
