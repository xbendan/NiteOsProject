#include <siberix/time/unit.h>

namespace Kern {
    class Date
    {
    public:
        Date()
          : m_years(0)
          , m_months(0)
          , m_days(0)
          , m_hours(0)
          , m_minutes(0)
          , m_seconds(0)
          , m_milliseconds(0)
          , m_daysOfWeek(0)
        {
        }

        Date(uint64_t years,
             uint8_t  months,
             uint8_t  days,
             uint8_t  hours,
             uint8_t  minutes,
             uint8_t  seconds,
             uint16_t milliseconds,
             uint8_t  daysOfWeek)
          : m_years(years)
          , m_months(months)
          , m_days(days)
          , m_hours(hours)
          , m_minutes(minutes)
          , m_seconds(seconds)
          , m_milliseconds(milliseconds)
          , m_daysOfWeek(daysOfWeek)
        {
        }

        Date(const Date& other)
          : m_years(other.m_years)
          , m_months(other.m_months)
          , m_days(other.m_days)
          , m_hours(other.m_hours)
          , m_minutes(other.m_minutes)
          , m_seconds(other.m_seconds)
          , m_milliseconds(other.m_milliseconds)
          , m_daysOfWeek(other.m_daysOfWeek)
        {
        }

        // clang-format off
        uint64_t years()        const { return m_years; }
        uint8_t  months()       const { return m_months; }
        uint8_t  days()         const { return m_days; }
        uint8_t  hours()        const { return m_hours; }
        uint8_t  minutes()      const { return m_minutes; }
        uint8_t  seconds()      const { return m_seconds; }
        uint16_t milliseconds() const { return m_milliseconds; }
        uint8_t  daysOfWeek()   const { return m_daysOfWeek; }
        // clang-format on

        uint64_t getAsTimestamp();

    private:
        uint64_t m_years;
        uint8_t  m_months;
        uint8_t  m_days;
        uint8_t  m_hours;
        uint8_t  m_minutes;
        uint8_t  m_seconds;
        uint16_t m_milliseconds;
        uint8_t  m_daysOfWeek;
    };
}