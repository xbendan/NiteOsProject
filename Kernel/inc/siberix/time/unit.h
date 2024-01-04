#pragma once

#include <stdcxx/types.h>

namespace Kern {
    enum class TimeUnit
    {
        Nanosecond,
        Microsecond,
        Millisecond,
        Second,
        Minute,
        Hour,
        Day,
        Month,
        Year
    };

    struct Duration
    {
        TimeUnit _span;
        uint64_t _amount;

        constexpr Duration(TimeUnit s, uint64_t a)
          : _span(s)
          , _amount(a)
        {
        }

        constexpr Duration(uint64_t ms)
          : _span(TimeUnit::Millisecond)
          , _amount(ms)
        {
        }

        uint64_t getAs(TimeUnit span)
        {
            switch (_span) {
                case TimeUnit::Nanosecond: {
                    return _amount / 1000000;
                }
                case TimeUnit::Microsecond: {
                    return _amount / 1000;
                }
                case TimeUnit::Millisecond: {
                    return _amount;
                }
                case TimeUnit::Second: {
                    return _amount * 1000;
                }
                case TimeUnit::Minute: {
                    return _amount * 1000 * 60;
                }
                case TimeUnit::Hour: {
                    return _amount * 1000 * 60 * 60;
                }
                case TimeUnit::Day: {
                    return _amount * 1000 * 60 * 60 * 24;
                }
                case TimeUnit::Month: {
                    return _amount * 1000 * 60 * 60 * 24 * 30;
                }
                case TimeUnit::Year: {
                    return _amount * 1000 * 60 * 60 * 24 * 365;
                }
            }
            return 0;
        }
    };
}