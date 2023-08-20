#pragma once

#include <siberix/core/time.h>
#include <siberix/device/connectivity.h>
#include <siberix/init/boot.h>
#include <siberix/mm/memory.h>
#include <siberix/mm/page.h>
#include <siberix/proc/process.h>
#include <siberix/proc/sched.h>

enum class Architecture {
    Undefined,
    X86_64,
    ARMv7,
    RISC_V
};

class KernelExecutive {
public:
    KernelExecutive(Architecture arch, BootConfig& bootConfig)
        : m_isInitialized(false),
          m_arch(arch),
          m_bootConfig(bootConfig) {}
    ~KernelExecutive();

    virtual bool setupArch() = 0;

    bool        isInitialized() { return m_isInitialized; }
    BootConfig& getBootConfig();

    MemoryService&      getMemory() { return m_memory; }
    DeviceConnectivity* getConnectivity() { return m_devices; }
    Scheduler*          getScheduler() { return m_scheduler; }

    bool addDevice(Device& device);
    bool checkDevice(Device& device);

    u64  getTimestamp();
    void sleep(u64 ms);

    Clock&       getClock();
    void         addTimer(TimerDevice& timer, bool setAsDefault = false);
    TimerDevice& getDefaultTimer();

protected:
    bool         m_isInitialized;
    Architecture m_arch;
    BootConfig&  m_bootConfig;

    MemoryService            m_memory;
    DeviceConnectivity*      m_devices;
    Scheduler*               m_scheduler;
    Clock                    m_clock;
    LinkedList<TimerDevice&> m_timers;
    TimerDevice*             m_defaultTimer;
};