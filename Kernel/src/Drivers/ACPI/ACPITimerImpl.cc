#include <Drivers/ACPI.h>

#include <Arch/x86_64/ports.h>

using namespace ACPI;

ACPITimer::ACPITimer()
{
    r_PMTTimerLength = g_FADT->PMTTimerLength;
    r_xPMTTimerBlock = &(g_FADT->xPMTTimerBlock);
    if (r_xPMTTimerBlock->Address)
    {
        switch (r_xPMTTimerBlock->AddressSpace)
        {
            case 0 /* Memory Space */: {
                m_TimerTicks = g_FADT->PMTTimerBlock;
                break;
            }
            default: {
                System::Out("Not implemented GAS Address space %u, reset to default.", r_xPMTTimerBlock->AddressSpace);
                m_TimerTicks = g_FADT->PMTTimerBlock;
                break;
            }
        }
    } 
    else
    {
        m_TimerTicks = g_FADT->PMTTimerBlock;
    }
}

ACPITimer::~ACPITimer()
{
    
}

void ACPITimer::Tick() { }

uint64_t ACPITimer::CurrentTime(TimeSpan span)
{
    return (uint64_t) Ports::ReadDword32(m_TimerTicks);
}

void ACPITimer::Sleep(long milliseconds)
{
    if (r_PMTTimerLength != 4)
    {
        return;
    }

    uint64_t clock = 3579545 * milliseconds / 1000;
    uint64_t counter = 0;
    uint64_t last = CurrentTime();
    uint64_t current = 0;

    while (counter < clock)
    {
        current = CurrentTime();
        if (current < last)
        {
            counter += ((g_FADT->Flags >> 8) & 0x1 ? 0x100000000ul : 0x1000000) + current - last;
        }
        else
        {
            counter += current - last;
        }
        last = current;
        asm("nop");
    }
}