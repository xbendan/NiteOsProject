#pragma once

#define KERNEL_VIRTUAL_BASE     0xffffffff80000000
#define IO_VIRTUAL_BASE         KERNEL_VIRTUAL_BASE - 0x100000000

#define USER_SPACE              0x0000000000000000
#define KERNEL_SPACE            0xFFFFFF8000000000

#define LOWMEM_RESERVED         0x100000

#include <macros>

class AddressSpace
{
protected:
    uint64_t m_AllocatedPages;
    uint64_t m_MappedPages;
    uintptr_t m_ZeroPage;
    void *m_VirtualPages;

public:
    AddressSpace();
    ~AddressSpace();

    inline uint64_t AllocatedPages() { return m_AllocatedPages; }
    inline uint64_t MappedPages() { return m_MappedPages; }
    inline void *VirtualPages() { return m_VirtualPages; }
    virtual uintptr_t Allocate4KPages(size_t amount);
    virtual void Free4KPages(uintptr_t address, size_t amount);
    virtual bool IsPagePresent(uintptr_t address);
    virtual uintptr_t ConvertVirtToPhys(uintptr_t address);

};

class KernelAddressSpace : public AddressSpace
{
public:
    KernelAddressSpace();
    ~KernelAddressSpace();

    uintptr_t Allocate4KPages(size_t amount) override;
    void Free4KPages(uintptr_t address, size_t amount) override;
    bool IsPagePresent(uintptr_t address) override;
    uintptr_t ConvertVirtToPhys(uintptr_t address) override;
};