#pragma once

#include <common/typedefs.h>
#include <utils/linked_list.h>

enum class TimeSpan {
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

struct Month {
    const char *fullname;
    const char *abbr;
    u8          index;
    u8          days;
};

struct Duration {
    TimeSpan span;
    u64      amount;
};

class Clock {
public:
    Clock();
    Clock(u32 s, u32 mi, u32 h, u32 d, u32 mo, u32 y)
        : seconds(s),
          minutes(mi),
          hours(h),
          days(d),
          months(mo),
          years(y) {}
    Clock(u64 timestamp) {
        seconds    = timestamp % 60;
        timestamp /= 60;
        minutes    = timestamp % 60;
        timestamp /= 60;
        hours      = timestamp % 24;
        timestamp /= 24;
        days       = timestamp % 30;
        timestamp /= 30;
        months     = timestamp % 12;
        timestamp /= 12;
        years      = timestamp;
    }
    ~Clock();

    u32 getSeconds() { return seconds; }
    u32 getMinutes() { return minutes; }
    u32 getHours() { return hours; }
    u32 getDays() { return days; }
    u32 getMonths() { return months; }
    u32 getYears() { return years; }
    u64 getAsTimestamp() {
        return (u64)seconds + (u64)minutes * 60 + (u64)hours * 3600 + (u64)days * 86400 +
               (u64)months * 2592000 + (u64)years * 31104000;
    }
    const char *getAsString();

private:
    u32 seconds;
    u32 minutes;
    u32 hours;
    u32 days;
    u32 months;
    u32 years;
};

extern Month months[12];