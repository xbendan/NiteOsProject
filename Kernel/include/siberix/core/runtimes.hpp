#include <siberix/init/boot.h>

#include <siberix/device/device_tree.hpp>
#include <siberix/mm/page.hpp>
#include <siberix/mm/service.hpp>
#include <siberix/proc/process.hpp>
#include <siberix/proc/sched.hpp>

enum CompiledArchitecture {
    CA_UNDEFINED = 0,
    CA_X86_64 = 1,
    CA_ARMV7 = 2,
    CA_RISCV = 2
};

class SystemRuntime {
public:
    SystemRuntime(CompiledArchitecture _type)
        : type(_type),
          initialized(false) {}
    ~SystemRuntime();

    virtual bool setup() = 0;
    virtual void loadMemory() = 0;
    virtual void loadDevices() = 0;
    virtual void shutdown(bool hw) = 0;
    virtual void reboot(bool hw) = 0;
    virtual void sleep() = 0;
    virtual void hibernate() = 0;

    bool isInitialized() { return m_isInitialized; }
    MemoryService& getMemory();
    BootConfig& getBootConfig();
    DeviceTree& getDeviceTree();
    Scheduler& getScheduler();

    const CompiledArchitecture arch;

protected:
    bool m_isInitialized;
    MemoryService m_memory;
    DeviceTree& m_devices;
    BootConfig& m_bootConfig;
    Scheduler& m_scheduler;
};

SystemRuntime* runtime();