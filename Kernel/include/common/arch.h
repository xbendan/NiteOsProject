#include <siberix/mm/page.hpp>

enum ArchType 
{
    CA_UNDEFINED = 0,
    CA_X86_64 = 1,
    CA_ARMV7 = 2,
    CA_RISCV = 2
};

class RuntimeSupport
{
public:
    RuntimeSupport(ArchType _type)
      : type(_type),
        initialized(false) { }

    virtual bool setup() = 0;
    virtual void loadMemory();
    virtual void loadDevices();
    ArchType getArchType();
    bool isInitialized() { return isInitialized; }

    Memory::PageAlloc *getPageAlloc() { return this->pageAlloc; }
    Memory::MemoryAlloc *getKernAlloc() { return this->kernelAlloc; }
    // void setPageAlloc(Memory::PageAlloc *pageAlloc) { this->pageAlloc = pageAlloc; }
    // void setKernAlloc(Memory::MemoryAlloc *kernAlloc) { this->kernelAlloc = kernAlloc; }

protected:
    const ArchType archType;
    bool initialized;
    Memory::PageAlloc *pageAlloc;
    Memory::MemoryAlloc *kernelAlloc;
};

RuntimeSupport& getRuntimeArch();