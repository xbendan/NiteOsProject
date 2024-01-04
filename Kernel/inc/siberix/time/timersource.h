#include <siberix/time/date.h>
#include <siberix/time/time.h>
#include <stdcxx/types.h>

namespace Kern::Time {
    class ITimerSource
    {
    public:
        virtual ~ITimerSource() = default;

        virtual uint64_t getTicks()               = 0;
        virtual void     sleep(uint64_t ms)       = 0;
        virtual void     sleep(Duration duration) = 0;
        virtual void     sleepUntil(Date date)    = 0;
    };
}