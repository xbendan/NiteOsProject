#pragma once

#include <siberix/time/unit.h>

namespace Kern {
    class Time
    {
    public:
        Time()
          : m_year(0)
          , m_month(0)
          , m_day(0)
          , m_hour(0)
          , m_minute(0)
          , m_second(0)
          , m_millisecond(0)
        {
        }

        Time(uint64_t year,
             uint64_t month,
             uint64_t day,
             uint64_t hour,
             uint64_t minute,
             uint64_t second,
             uint64_t millisecond)
          : m_year(year)
          , m_month(month)
          , m_day(day)
          , m_hour(hour)
          , m_minute(minute)
          , m_second(second)
          , m_millisecond(millisecond)
        {
        }

        Time(const Time& other)
          : m_year(other.m_year)
          , m_month(other.m_month)
          , m_day(other.m_day)
          , m_hour(other.m_hour)
          , m_minute(other.m_minute)
          , m_second(other.m_second)
          , m_millisecond(other.m_millisecond)
        {
        }

        Time(uint64_t timestamp) {}

        Time& operator=(const Time& other)
        {
            m_year        = other.m_year;
            m_month       = other.m_month;
            m_day         = other.m_day;
            m_hour        = other.m_hour;
            m_minute      = other.m_minute;
            m_second      = other.m_second;
            m_millisecond = other.m_millisecond;
            return *this;
        }

        bool operator==(const Time& other)
        {
            return m_year == other.m_year && m_month == other.m_month &&
                   m_day == other.m_day && m_hour == other.m_hour &&
                   m_minute == other.m_minute && m_second == other.m_second &&
                   m_millisecond == other.m_millisecond;
        }

        Time& operator+=(const Time& other)
        {
            m_year        += other.m_year;
            m_month       += other.m_month;
            m_day         += other.m_day;
            m_hour        += other.m_hour;
            m_minute      += other.m_minute;
            m_second      += other.m_second;
            m_millisecond += other.m_millisecond;
            fixOverflow();
            return *this;
        }

        void fixOverflow()
        {
            if (m_millisecond > 999) {
                m_second      += m_millisecond / 1000;
                m_millisecond %= 1000;
            }
            if (m_second > 59) {
                m_minute += m_second / 60;
                m_second %= 60;
            }
            if (m_minute > 59) {
                m_hour   += m_minute / 60;
                m_minute %= 60;
            }
            if (m_hour > 23) {
                m_day  += m_hour / 24;
                m_hour %= 24;
            }
            if (m_day > 30) {
                m_month += m_day / 30;
                m_day   %= 30;
            }
            if (m_month > 12) {
                m_year  += m_month / 12;
                m_month %= 12;
            }
        }

        Duration getAs(TimeUnit unit);

    private:
        uint64_t m_year;
        uint64_t m_month;
        uint64_t m_day;
        uint64_t m_hour;
        uint64_t m_minute;
        uint64_t m_second;
        uint64_t m_millisecond;
    };
}