#include <arch/x86_64/apic.h>
#include <siberix/core/runtimes.h>

ApicTimerDevice::ApicTimerDevice(ApicLocalInterface& interface) {
    u64 hertz = 1000;
    u32 irq   = 0x20;
    u32 t     = 0xffffffff;

    interface.write(LOCAL_APIC_LVT_TIMER, 0x10000);
    interface.write(LOCAL_APIC_TIMER_DIVIDE, 0x3);
    interface.write(LOCAL_APIC_LVT_TIMER, 0x0);

    interface.write(LOCAL_APIC_TIMER_INITIAL_COUNT, t);
    siberix()->getDefaultTimer().sleep(100);
    interface.write(LOCAL_APIC_LVT_TIMER, 0x10000);

    m_busClock = (t - currentTime()) * 0x10 * 10;

    interface.write(LOCAL_APIC_LVT_TIMER, 0x00020000 | irq);
    interface.write(LOCAL_APIC_TIMER_DIVIDE, 0x3);
    interface.write(LOCAL_APIC_TIMER_INITIAL_COUNT, m_busClock / 16 / hertz);
}

ApicTimerDevice::~ApicTimerDevice() {}