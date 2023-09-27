#include <siberix/core/time.h>
#include <siberix/device/types.h>

class IntervalTimerDevice : public TimerDevice
{
public:
    IntervalTimerDevice(u32 freq);
    ~IntervalTimerDevice();

    void     sleep(Duration duration) override;
    void     sleep(u64 ms) override;
    Duration time() override;
    u32      getFrequency();

    static void doTick();

private:
    u32 m_frequency;

    static inline volatile u64 m_ticks;
};