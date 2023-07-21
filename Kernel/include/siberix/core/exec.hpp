#include <siberix/init/boot.h>

#include <siberix/core/time.hpp>
#include <siberix/device/connectivity.hpp>
#include <siberix/mm/page.hpp>
#include <siberix/mm/service.hpp>
#include <siberix/proc/process.hpp>
#include <siberix/proc/sched.hpp>

enum CompiledArchitecture {
    CA_UNDEFINED = 0,
    CA_X86_64    = 1,
    CA_ARMV7     = 2,
    CA_RISCV     = 3
};

class KernelExecutive {
public:
    KernelExecutive(CompiledArchitecture arch)
        : m_arch(arch),
          m_isInitialized(false) {}
    ~KernelExecutive();

    virtual bool setupArch() = 0;

    bool        isInitialized() { return m_isInitialized; }
    BootConfig& getBootConfig();

    MemoryManagement&   getMemory() { return m_memory; }
    DeviceConnectivity& getConnectivity() { return m_devices; }
    Scheduler&          getScheduler() { return m_scheduler; }
    TimeManagement&     getTimeManagement() { return m_clockAndTime; }

    bool addDevice(Device& device);
    bool checkDevice(Device& device);

protected:
    bool                 m_isInitialized;
    CompiledArchitecture m_arch;
    BootConfig&          m_bootConfig;

    AddressSpace*       m_kernelSpace;
    TimeManagement      m_clockAndTime;
    MemoryManagement    m_memory;
    DeviceConnectivity& m_devices;
    Scheduler&          m_scheduler;
};

KernelExecutive* exec();