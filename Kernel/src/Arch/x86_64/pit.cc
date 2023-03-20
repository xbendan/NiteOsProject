#include <Arch/x86_64/pit.h>
#include <Arch/x86_64/ports.h>
#include <Arch/x86_64/cpu.h>
#include <Arch/x86_64/irq.h>
#include <Timer.h>

using namespace Ports;

namespace PIT
{
    void TimerTickHandler(void *data, registers_t *regs) {
        //Timer::Tick();
    }

    void Initialize(uint32_t frequency)
    {
        uint16_t __divisor__ = PIT_FREQUENCY / frequency;

        WriteByte8(0x43, 0x36);
        WriteByte8(0x40, __divisor__ & 0xFF);
        WriteByte8(0x40, (__divisor__ >> 8) & 0xFF);

        RegisterInterruptHandler(32, TimerTickHandler);
    }
} // namespace PIT

// namespace Timer
// {
//     int frequency = 1000;
//     volatile uint64_t ticks = 0;
//     uint64_t uptimeUs = 0;
//     long pendingTicks = 0;
    
//     void Tick() { ticks++; }
//     void Sleep(long milliseconds) {
//         uint64_t futureTicks = ticks + milliseconds;
//         while (ticks < futureTicks)
//             asm volatile("pause");
//     }
// }