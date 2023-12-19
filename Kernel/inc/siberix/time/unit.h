#include <stdcxx/types.h>

namespace Kern::Time {
    enum class TimeSpan
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
        TimeSpan _span;
        UInt64   _amount;

        constexpr Duration(TimeSpan s, UInt64 a)
          : _span(s)
          , _amount(a)
        {
        }

        constexpr Duration(UInt64 ms)
          : _span(TimeSpan::Millisecond)
          , _amount(ms)
        {
        }

        UInt64 getAs(TimeSpan span)
        {
            switch (_span) {
                case TimeSpan::Nanosecond: {
                    return _amount / 1000000;
                }
                case TimeSpan::Microsecond: {
                    return _amount / 1000;
                }
                case TimeSpan::Millisecond: {
                    return _amount;
                }
                case TimeSpan::Second: {
                    return _amount * 1000;
                }
                case TimeSpan::Minute: {
                    return _amount * 1000 * 60;
                }
                case TimeSpan::Hour: {
                    return _amount * 1000 * 60 * 60;
                }
                case TimeSpan::Day: {
                    return _amount * 1000 * 60 * 60 * 24;
                }
                case TimeSpan::Month: {
                    return _amount * 1000 * 60 * 60 * 24 * 30;
                }
                case TimeSpan::Year: {
                    return _amount * 1000 * 60 * 60 * 24 * 365;
                }
            }
            return 0;
        }
    };
}