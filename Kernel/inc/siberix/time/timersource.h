#include <stdcxx/types.h>

namespace Kern::Time {
    class TimerSource
    {
    public:
        virtual ~TimerSource() = default;

        virtual UInt64 getTicks()     = 0;
        virtual UInt64 getFrequency() = 0;
    };
}