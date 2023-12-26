#include <siberix/mem/address-space.h>
#include <siberix/mem/kmem-alloc.h>
#include <siberix/mem/page.h>
#include <stdcxx/types.h>

namespace Kern::Mem {
    uint64_t copyAsIoAddress(uint64_t address);

    class IMemHost
    {
    public:
        IMemHost();

        uint64_t      alloc4KPages(uint64_t amount);
        uint64_t      alloc4KPages(uint64_t amount, AddressSpace* addressSpace);
        void          free4KPages(void* address, uint64_t amount);
        IMemAlloc*    getKernMemAlloc() const;
        IMemAlloc*    getPageMemAlloc() const;
        AddressSpace* getKernelAddressSpace() const;

    private:
        IMemAlloc*    m_kmalloc;
        IMemAlloc*    m_physAlloc;
        AddressSpace* m_kernelAddressSpace;
    };
}