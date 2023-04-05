#define KERNEL_VIRTUAL_BASE     0xffffffff80000000
#define IO_VIRTUAL_BASE         KERNEL_VIRTUAL_BASE - 0x100000000

#define USER_SPACE              0x0000000000000000
#define KERNEL_SPACE            0xFFFFFF8000000000

#define LOWMEM_RESERVED         0x100000

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

    virtual uintptr_t Allocate4KPages(size_t amount, void **page = nullptr);
    virtual void Free4KPages(uintptr_t address, size_t amount);
};

class KernelAddressSpace : public AddressSpace
{
public:
    KernelAddressSpace();
    ~KernelAddressSpace();

    uintptr_t Allocate4KPages(size_t amount, void **page = nullptr) override;
    void Free4KPages(uintptr_t address, size_t amount) override;
};