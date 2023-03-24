#include <Drivers/Generic/ACPI.h>
#include <Drivers/Clock/HPET.h>
#include <Timer.h>
#include <System.h>

HPETTimer::HPETTimer()
{
    if ((r_HPET = ACPI::g_HPET) == nullptr)
    {
        System::Out("Cannot initialize HPET Timer without HPET structure.");
        return;
    }
    else
        System::Out("Initializing HPET, address=%x", r_HPET->Address.Address);

    // clock = Read(0) >> 32;
    // Write(0x10, 0);
    // Write(0xf0, 0);
    // Write(0x10, 1);
}

void HPETTimer::Tick() { }

uint64_t HPETTimer::CurrentTime(TimeSpan span)
{
    return Read(0xF0);
}

void HPETTimer::Sleep(long milliseconds)
{
    uint64_t futureTicks = CurrentTime() + (milliseconds * 10000000000000) / clock;
    while (CurrentTime() < futureTicks) asm("nop");
}