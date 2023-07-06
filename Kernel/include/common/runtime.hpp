#include <siberix/init/boot.h>
#include <siberix/mm/page.hpp>
#include <siberix/mm/service.hpp>

enum ArchType { CA_UNDEFINED = 0, CA_X86_64 = 1, CA_ARMV7 = 2, CA_RISCV = 2 };

class SystemRuntime {
public:
    SystemRuntime(ArchType _type) : type(_type), initialized(false) {}

    virtual bool setup() = 0;
    virtual void loadMemory();
    virtual void loadDevices();
    ArchType getArchType();
    bool isInitialized() { return isInitialized; }
    MemoryService& memory();
    BootConfig& getBootConfig();

protected:
    const ArchType archType;
    bool initialized;
    MemoryService memoryService;
    BootConfig& bootConfig;
};

SystemRuntime* runtime();

void* operator new(size_t size) { return (void*)runtime()->memory().alloc(size); }

void* operator new[](size_t size) { return (void*)runtime()->memory().alloc(size); }

void operator delete(void* p) { runtime()->memory().free((u64)p); }

void operator delete(void* p, size_t) { ::operator delete(p); }

void operator delete[](void* p) { runtime()->memory().free((u64)p); }

void operator delete[](void* p, size_t) { ::operator delete[](p); }