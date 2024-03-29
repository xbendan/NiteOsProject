#include <Drivers/Generic/APIC.h>
#include <System.h>
#include <Timer.h>

#include <Arch/x86_64/Interrupts.h>

using namespace APIC::Local;

void ApicTimerDevice::TimerEvent(Interrupt *data, RegisterContext *regs)
{
    System::Out("Tick!");
    g_Timers[TimerAPIC]->Tick();
}

uint32_t ApicTimerDevice::EstimateBusSpeed()
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

ApicTimerDevice::ApicTimerDevice()
{
    uint64_t hertz = 1000;
    uint32_t irq = 0x20;
    m_BusClock = EstimateBusSpeed();
    
    WriteData(LOCAL_APIC_LVT_TIMER, 0x00020000 | irq);
    WriteData(LOCAL_APIC_TIMER_DIVIDE, 0x3);
    WriteData(LOCAL_APIC_TIMER_INITIAL_COUNT, m_BusClock / 16 / hertz);

    APIC::IO::EnableInterrupt(irq);
    RegisterIRQ(irq, TimerEvent);
}

ApicTimerDevice::~ApicTimerDevice()
{
    WriteData(LOCAL_APIC_LVT_TIMER, 0x00020000 | 0x10000);
}

void ApicTimerDevice::Tick() { m_Ticks++; }

uint64_t ApicTimerDevice::CurrentTime(TimeSpan span)
{
    // return ReadData(LOCAL_APIC_TIMER_CURRENT_COUNT) * 1000 / span;
    return m_Ticks * 1000 / span;
}

void ApicTimerDevice::Sleep(long milliseconds)
{

}