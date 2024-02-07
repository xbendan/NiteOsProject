#pragma once

#include <stdcxx/type-traits.h>
#include <stdcxx/types.h>

namespace Std {
    template <typename N>
        requires isIntegral<N>
    struct Range
    {
        N _min;
        N _max;

        constexpr Range() = default;

        constexpr Range(N min, N max)
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