#include <siberix/core/time.hpp>

enum PowerState { PowerStandby, PowerHibernate, PowerSleep };

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