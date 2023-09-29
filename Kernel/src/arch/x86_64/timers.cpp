#include <arch/x86_64/arch.h>
#include <arch/x86_64/clocks.h>
#include <arch/x86_64/interrupts.h>
#include <arch/x86_64/iopt.h>
#include <common/logger.h>

void
TimerTickEvent(RegisterContext* regs)
{
    IntervalTimerDevice::doTick();
}

void
IntervalTimerDevice::doTick()
{
    m_ticks += 1;
}

IntervalTimerDevice::IntervalTimerDevice(u32 freq)
  : TimerDevice("Programmable Interval Timer Device")
  , m_frequency(freq)
{
    static_cast<SbrxkrnlX64Impl*>(siberix())->getInterrupt(0x20).ifPresent(
      [](Interrupt& interrupt) -> void {
          interrupt.setExecutor(TimerTickEvent);
      });

    u32 divisor = 1193182 / freq;
    u8  l       = divisor & 0xff;
    u8  h       = divisor >> 8;
    outByte8(0x43, 0x36);
    outByte8(0x40, l);
    outByte8(0x40, h);
}

void
IntervalTimerDevice::sleep(Duration duration)
{
    sleep(duration.as(TimeSpan::Millisecond));
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