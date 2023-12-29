#include <stdcxx/types.h>

namespace Kern::Time {
    class ITimerSource
    {
    public:
        virtual ~ITimerSource() = default;

        virtual uint64_t getTicks()     = 0;
        virtual uint64_t getFrequency() = 0;
    };
}