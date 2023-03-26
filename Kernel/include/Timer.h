#pragma once

#include <macros>

enum TimeSpan
{
    Microsecond = 1,
    Millisecond = 1000,
    Second = 1000000,
    Minute = 60000000,
    Hour = 3600000000,
    Day = 86400000000,
    Month = 2592000000000,
    Year = 31536000000000,
};

enum TimerType
{
    TimerPIT,
    TimerACPI,
    TimerAPIC,
    TimerRTC,
    TimerHPET
};

class Timer
{
public:
    static TimerType g_TypePreferrence;

    virtual void Tick() = 0;
    virtual uint64_t CurrentTime(TimeSpan span = Millisecond) = 0;
    virtual void Sleep(long milliseconds) = 0;
};

extern Timer *g_Timers[5];

void Sleep(long milliseconds);