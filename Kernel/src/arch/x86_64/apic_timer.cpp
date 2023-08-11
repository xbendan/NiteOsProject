#include <arch/x86_64/apic.h>

ApicTimerDevice::ApicTimerDevice(ApicLocalInterface& interface) {
    uint64_t hertz = 1000;
    uint32_t irq   = 0x20;
    uint32_t t     = 0xffffffff;

    interface.write(LOCAL_APIC_LVT_TIMER, 0x10000);
    interface.write(LOCAL_APIC_TIMER_DIVIDE, 0x3);
    interface.write(LOCAL_APIC_LVT_TIMER, 0x0);

    interface.write(LOCAL_APIC_TIMER_INITIAL_COUNT, t);
    // g_Timers[TimerACPI]->Sleep(100);
    interface.write(LOCAL_APIC_LVT_TIMER, 0x10000);

    m_busClock = (t - currentTime()) * 0x10 * 10;

    interface.write(LOCAL_APIC_LVT_TIMER, 0x00020000 | irq);
    interface.write(LOCAL_APIC_TIMER_DIVIDE, 0x3);
    interface.write(LOCAL_APIC_TIMER_INITIAL_COUNT, m_BusClock / 16 / hertz);
}

ApicTimerDevice::~ApicTimerDevice() {}