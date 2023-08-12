#pragma once

#include <siberix/core/time.h>
#include <siberix/device/connectivity.h>
#include <siberix/init/boot.h>
#include <siberix/mm/page.h>
#include <siberix/mm/service.h>
#include <siberix/proc/process.h>
#include <siberix/proc/sched.h>

enum CompiledArchitecture {
    CA_UNDEFINED = 0,
    CA_X86_64    = 1,
    CA_ARMV7     = 2,
    CA_RISCV     = 3
};

class KernelExecutive {
public:
    KernelExecutive(CompiledArchitecture arch, BootConfig& bootConfig)
        : m_isInitialized(false),
          m_arch(arch),
          m_bootConfig(bootConfig) {}
    ~KernelExecutive();

    virtual bool setupArch() = 0;

    bool        isInitialized() { return m_isInitialized; }
    BootConfig& getBootConfig();

    MemoryManagement&   getMemory() { return m_memory; }
    DeviceConnectivity& getConnectivity() { return m_devices; }
    Scheduler&          getScheduler() { return m_scheduler; }

    bool addDevice(Device& device);
    bool checkDevice(Device& device);

    u64          getTimestamp();
    Clock&       getClock();
    void         addTimer(TimerDevice& timer, bool setAsDefault = false);
    TimerDevice& getDefaultTimer();

protected:
    bool                 m_isInitialized;
    CompiledArchitecture m_arch;
    BootConfig&          m_bootConfig;

    AddressSpace*       m_kernelSpace;
    MemoryManagement    m_memory;
    DeviceConnectivity& m_devices;
    Scheduler&          m_scheduler;

    Clock                    m_clock;
    LinkedList<TimerDevice&> m_timers;
};

KernelExecutive* exec();