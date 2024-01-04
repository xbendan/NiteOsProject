#include <drivers/apic/device.h>
#include <siberix/dvc/device.h>
#include <siberix/time/timersource.h>

namespace Kern::Hal::Impls {
    class ApicTimerDevice
      : Time::ITimerSource
      , IDevice
    {
    public:
        ApicTimerDevice(ApicDevice::ApicLocal* local)
          : IDevice("APIC Timer", DeviceType::TimerOrClock)
          , m_local(local)
        {
        }

        void sleep(uint64_t ms) override;
        void sleep(Duration duration) override;
        void sleepUntil(Date date) override;

    private:
        ApicDevice::ApicLocal* m_local;
        uint64_t           m_clockFreq;
        volatile uint64_t  m_clockTicks;
    };
}