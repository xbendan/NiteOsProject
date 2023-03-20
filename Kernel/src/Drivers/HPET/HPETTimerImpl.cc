#include <Drivers/ACPI.h>
#include <Drivers/HPET.h>
#include <Timer.h>
#include <System.h>

HPETTimer::HPETTimer()
{
    if ((r_HPET = ACPI::g_HPET) == nullptr)
    {
        System::Out("Cannot initialize HPET Timer without HPET structure.");
        return;
    }

    clock = Read(0) >> 32;
    Write(0x10, 0);
    Write(0xf0, 0);
    Write(0x10, 1);
}

void HPETTimer::Tick() { }

uint64_t HPETTimer::CurrentTime(TimeSpan span)
{
    return 0;
}

void HPETTimer::Sleep(long milliseconds)
{
    ticks = 0;
    uint64_t futureTicks = ticks + (milliseconds * 10000000000000) / clock;
    while (ticks < futureTicks) asm("nop");
}