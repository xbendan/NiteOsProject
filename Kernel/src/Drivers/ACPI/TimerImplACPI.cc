#include <Drivers/ACPI.h>

#include <Arch/x86_64/ports.h>

using namespace ACPI;

TimerImplACPI::TimerImplACPI()
{
    r_PMTTimerLength = g_FADT->PMTTimerLength;
    r_xPMTTimerBlock = &(g_FADT->xPMTTimerBlock);
    if (r_xPMTTimerBlock->Address)
    {
        switch (r_xPMTTimerBlock->addressSpace)
        {
            case 0 /* Memory Space */: {

            }
            default: {

            }
        }
    } 
    else
    {
        
    }
}

TimerImplACPI::~TimerImplACPI()
{
    
}

void TimerImplACPI::Tick() { }

uint64_t TimerImplACPI::CurrentTime(TimeSpan span = Millisecond)
{
    return Ports::ReadDword32(m_TimerTicks / span);
}

void TimerImplACPI::Sleep(long milliseconds)
{
    if (r_PMTTimerLength != 4)
    {
        return;
    }

    uint64_t clock = 3579545 * microsecs / 1000000;
    uint64_t counter = 0;
    uint64_t last = CurrentTime();
    uint64_t current = 0;

    while (counter < clock)
    {
        current = CurrentTime();
        if (current < last)
        {
            counter += ((acpiFadt->flags >> 8) & 0x1 ? 0x100000000ul : 0x1000000) + current - last;
        }
        else
        {
            counter += current - last;
        }
        last = current;
        asm("nop");
    }
}