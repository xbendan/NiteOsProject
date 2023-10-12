#pragma once

#include <siberix/core/instructions.h>
#include <siberix/core/power.h>
#include <siberix/core/time.h>
#include <siberix/device/connectivity.h>
#include <siberix/init/boot.h>
#include <siberix/mm/memory.h>
#include <siberix/mm/page.h>
#include <siberix/proc/process.h>
#include <siberix/proc/sched.h>

#include <utils/result.h>

enum class Architecture
{
    Undefined,
    X86_64,
    ARMv7,
    RISC_V
};

class TimeNClock
{
public:
    Clock&                       clock();
    utils::Optional<TimerDevice> timer();
    Result<Void> addTimer(TimerDevice* device, bool setAsDefault = false)
    {
        if (device == nullptr) {
            return Result<Void>(ResultResponse::NullPointerException);
        }

        m_timers.add(device);
        if (setAsDefault) {
            m_defaultTimer = device;
        }
    }
    u64  getTimestamp();
    void sleep(u64) {}

private:
    Clock                    m_clock;
    LinkedList<TimerDevice*> m_timers;
    TimerDevice*             m_defaultTimer;
};

class KernelComponents
{
public:
    KernelComponents(Architecture arch, BootConfig& bootConfig)
      : m_isInitialized(false)
      , m_arch(arch)
      , m_bootConfig(bootConfig)
    {
    }
    ~KernelComponents() {}

    bool setupArch();

    bool        isInitialized() { return m_isInitialized; }
    BootConfig& getBootConfig();

    MemoryServiceProvider& getMemory() { return m_memory; }
    DeviceConnectivity*    getConnectivity() { return m_devices; }
    Scheduler*             getScheduler() { return m_scheduler; }
    TimeNClock&            getTimeNClock() { return m_timeNClocks; }

    Process* getKernelProcess();

protected:
    bool         m_isInitialized;
    Architecture m_arch;
    BootConfig&  m_bootConfig;

    MemoryServiceProvider m_memory;
    PowerEngine           m_energy;
    DeviceConnectivity*   m_devices;
    Scheduler*            m_scheduler;
    TimeNClock            m_timeNClocks;

    utils::LinkedList<InstructionSet>                        m_instructionSets;
    utils::Collection<siberix::hal::LogicalProcessingUnit*>* m_cpus;
};

KernelComponents*
kern();