#include <Drivers/APIC.h>
#include <System.h>
#include <Timer.h>

using namespace APIC::Local;

uint32_t LocalAPICTimer::EstimateBusSpeed()
{
    WriteData(LOCAL_APIC_LVT_TIMER, 0x10000);
    WriteData(LOCAL_APIC_TIMER_DIVIDE, 0x3);
    WriteData(LOCAL_APIC_LVT_TIMER, 0);

    uint32_t t = 0xFFFFFFFF;

    WriteData(LOCAL_APIC_TIMER_INITIAL_COUNT, t);
    g_Timers[TimerACPI]->Sleep(100);
    WriteData(LOCAL_APIC_LVT_TIMER, 0x10000);
    
    uint64_t frequency = (t - CurrentTime()) * 0x10 * 10;
    return frequency;
}

LocalAPICTimer::LocalAPICTimer()
{
    uint64_t hertz = 1000;
    uint32_t irq = 0x20;
    m_BusClock = EstimateBusSpeed();
    
    WriteData(LOCAL_APIC_LVT_TIMER, 0x00020000 | irq);
    WriteData(LOCAL_APIC_TIMER_DIVIDE, 0x3);
    WriteData(LOCAL_APIC_TIMER_INITIAL_COUNT, m_BusClock / 16 / hertz);

    APIC::IO::EnableInterrupt(irq);
}

LocalAPICTimer::~LocalAPICTimer()
{
    WriteData(LOCAL_APIC_LVT_TIMER, 0x00020000 | 0x10000);
}

void LocalAPICTimer::Tick() { }

uint64_t LocalAPICTimer::CurrentTime(TimeSpan span)
{
    return ReadData(LOCAL_APIC_TIMER_CURRENT_COUNT) * 1000 / span;
}

void LocalAPICTimer::Sleep(long milliseconds)
{

}