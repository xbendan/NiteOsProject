#include <siberix/time/unit.h>

namespace Kern {
    class Calendar
    {
    public:
        enum class Month
        {
            January   = 1,
            February  = 2,
            March     = 3,
            April     = 4,
            May       = 5,
            June      = 6,
            July      = 7,
            August    = 8,
            September = 9,
            October   = 10,
            November  = 11,
            December  = 12
        };

        static uint8_t getDaysInMonth(Month month, uint64_t year)
        {
            switch (month) {
                case Month::January:
                case Month::March:
                case Month::May:
                case Month::July:
                case Month::August:
                case Month::October:
                case Month::December: {
                    return 31;
                }
                case Month::April:
                case Month::June:
                case Month::September:
                case Month::November: {
                    return 30;
                }
                case Month::February: {
                    return isLeapYear(year) ? 29 : 28;
                }
                default: {
                    return 0;
                }
            }
        }

        static constexpr bool isLeapYear(uint64_t year)
        {
            return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
        }
    };
}