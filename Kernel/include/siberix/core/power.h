#include <siberix/core/time.h>

#define ENERGY_POLICY_ALLOW_HIBERNATE 0x1
#define ENERGY_POLICY_ALLOW_SLEEP 0x2
#define ENERGY_POLICY_ALLOW_COUNTER_RESET 0x4
#define ENERGY_POLICY_DISABLE_PROGRAMMED_SHUTDOWN 0x8
#define ENERGY_POLICY_FORCE_SHUTDOWN_COUNTDOWN 0x10
#define ENERGY_POLICY_FORCE_SHTUDOWN_NOTIFY 0x20

enum class EnergyState {
    Standby,
    Hibernate,
    Sleep
};

enum PowerPolicy {
    HIBERNATE_ENABLED,
    SLEEP_ENABLED,
    ALLOW_COUNTER_RESET,
};

class SleepCounter {
public:
    /// @brief Reset the sleep counter to zero
    void reset();

private:
    /// @brief Will the system fall into hibernation or sleep mode automatically
    bool     m_isEnabled;
    /// @brief How long have been passed away since the last operation
    u64      m_counter;
    /// @brief The duration to hibernate after the last operation on operating system
    Duration m_hibernateTime;
    /// @brief The duration to sleep after the last operation on operating system
    Duration m_sleepTime;
};

class PowerEngine {
public:
    PowerEngine();
    ~PowerEngine();

    void          setEnergyState(EnergyState state);
    EnergyState   getEnergyState();
    void          shutdown();
    void          reboot();
    bool          isBatteryContained();
    SleepCounter* getSleepCounter();
    void          setExecutionPolicy();

private:
    EnergyState m_state;
    u64         m_sleepCounter;
};