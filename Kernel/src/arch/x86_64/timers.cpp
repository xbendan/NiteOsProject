#include <arch/x86_64/arch.h>
#include <arch/x86_64/clocks.h>
#include <arch/x86_64/interrupts.h>
#include <arch/x86_64/iopt.h>

void
TimerTickEvent(RegisterContext* regs)
{
    IntervalTimerDevice::doTick();
}

void
IntervalTimerDevice::doTick()
{
    m_ticks++;
}

IntervalTimerDevice::IntervalTimerDevice(u32 freq)
  : m_frequency(freq)
  , TimerDevice("Programmable Interval Timer Device")
{
    static_cast<SbrxkrnlX64Impl*>(siberix())
      ->getInterrupt(0x20)
      .get()
      ->setExecutor(TimerTickEvent);

    u32 divisor = 1193182 / freq;
    u8  l = divisor & 0xff, h = divisor >> 8;
    outByte8(0x43, 0x36);
    outByte8(0x40, l);
    outByte8(0x40, h);
}

void
IntervalTimerDevice::sleep(Duration duration)
{
}

void
IntervalTimerDevice::sleep(u64 ms)
{
    if (ms) {
        u64 futureTicks = ms + m_ticks;
        while (m_ticks <= futureTicks)
            asm volatile("pause");
    }
}

Duration
IntervalTimerDevice::time()
{
    return Duration(m_ticks);
}