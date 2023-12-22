#include <stdcxx/types.h>

namespace Kern::Time {
    class TimerSource
    {
    public:
        virtual ~TimerSource() = default;

        virtual uint64_t getTicks()     = 0;
        virtual uint64_t getFrequency() = 0;
    };
}