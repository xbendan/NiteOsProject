#include <siberix/core/time.h>

enum class PowerState {
    Standby,
    Hibernate,
    Sleep
};

class SleepCounter {
public:
    void reset();

private:
    bool m_isEnabled;
    u64  m_counter;
};

class PowerManagement {
public:
    void         setPowerState(PowerState state);
    PowerState   getPowerState();
    virtual void shutdown() = 0;
    virtual void reboot()   = 0;
    void         setSleepAfter(Duration duration);

private:
    PowerState m_state;
    Duration   m_sleepAfter;
    u64        m_sleepCounter;
};