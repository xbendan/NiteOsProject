#include <common/typedefs.h>

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

class TimerDevice {
public:
    TimerDevice(const char *name)
        : m_name(name) {}
    ~TimerDevice() {}

    virtual void sleep(Duration duration) = 0;
    virtual void sleep(u32 ms)            = 0;

    const char *getName() { return m_name; }
    bool        isDefault() { return m_isDefault; }
    void        setDefault(bool isDef) { m_isDefault = isDef; }
    u8          getId() { return m_id; }

private:
    const char *m_name;
    bool        m_isDefault;
    u8          m_id;
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
    ~Clock();

    u32         getSeconds() { return seconds; }
    u32         getMinutes() { return minutes; }
    u32         getHours() { return hours; }
    u32         getDays() { return days; }
    u32         getMonths() { return months; }
    u32         getYears() { return years; }
    u64         getAsTimestamp();
    const char *getAsString();

private:
    u32 seconds;
    u32 minutes;
    u32 hours;
    u32 days;
    u32 months;
    u32 years;
};

class TimeManagement {
public:
    Clock &getClock();
    u64    getTimestamp();
    void   addTimer(TimerDevice &timer, bool setAsDefault = false);
    void   setDefaultTimer(TimerDevice &timer);
    LinkedList<TimerDevice &> &getTimers();

private:
    Clock                     m_clock;
    LinkedList<TimerDevice &> m_timers;
};

extern Month months[12];