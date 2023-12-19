#include <stdcxx/concepts.h>
#include <stdcxx/types.h>

namespace Std {
    template <typename N>
        requires Integral<N>
    struct Range
    {
        N _min;
        N _max;

        Range(N min, N max)
          : _min(min)
          , _max(max)
        {
        }

        N constraintToRange(N value)
        {
            if (value < _min)
                return _min;
            if (value > _max)
                return _max;
            return value;
        }

        bool isInRange(N value) { return value >= _min && value <= _max; }
    };
}