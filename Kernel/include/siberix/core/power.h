#include <siberix/core/time.h>

enum class EnergyState {
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

class EnergyPolicyEngine {
public:
    EnergyPolicyEngine();
    ~EnergyPolicyEngine();

    void        setPowerState(EnergyState state);
    EnergyState getPowerState();
    void        shutdown();
    void        reboot();
    void        setSleepAfter(Duration duration);

private:
    EnergyState m_state;
    Duration    m_sleepAfter;
    u64         m_sleepCounter;
};