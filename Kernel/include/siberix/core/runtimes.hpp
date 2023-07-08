#include <siberix/device/device_tree.hpp>
#include <siberix/init/boot.h>
#include <siberix/mm/page.hpp>
#include <siberix/mm/service.hpp>

enum CompiledArchitecture { CA_UNDEFINED = 0, CA_X86_64 = 1, CA_ARMV7 = 2, CA_RISCV = 2 };

class SystemRuntime {
public:
    SystemRuntime(CompiledArchitecture _type) : type(_type), initialized(false) {}
    ~SystemRuntime();

    virtual bool setup() = 0;
    virtual void loadMemory() = 0;
    virtual void loadDevices() = 0;
    virtual void shutdown(bool hw) = 0;
    virtual void reboot(bool hw) = 0;
    virtual void sleep() = 0;
    virtual void hibernate() = 0;

    bool isInitialized() { return initialized; }
    MemoryService& getMemory();
    BootConfig& getBootConfig();
    DeviceTree* getDeviceTree();

    const CompiledArchitecture arch;

protected:
    bool initialized;
    MemoryService memoryService;
    DeviceTree* deviceTree;
    BootConfig& bootConfig;
};

SystemRuntime* runtime();