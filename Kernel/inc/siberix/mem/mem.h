#include <siberix/mem/address-space.h>
#include <siberix/mem/kmem-alloc.h>
#include <siberix/mem/page.h>
#include <stdcxx/types.h>
#include <xtra-math/align.h>

namespace Kern::Mem {
    uint64_t copyAsIoAddress(uint64_t address);

    class IMemAlloc
    {
    public:
        virtual uint64_t alloc(uint64_t size) = 0;
        virtual void     free(void* address)  = 0;
    };

    using namespace Xtra::Math;

    class IPageAlloc : private IMemAlloc
    {
    public:
        virtual uint64_t allocatePhysMemory4K(uint64_t amount)      = 0;
        virtual Page4K*  allocatePhysMemory4KPages(uint64_t amount) = 0;
        virtual void     freePhysMemory4K(uint64_t address)         = 0;
        virtual void     freePhysMemory4KPages(Page4K* page)        = 0;

    private:
        friend class IMemoryHost;
        uint64_t alloc(uint64_t size) override
        {
            return allocatePhysMemory4K(alignUp(size, PAGE_SIZE_4K) /
                                        PAGE_SIZE_4K);
        }

        void free(void* address) override
        {
            freePhysMemory4K((uint64_t)address);
        }
    };

    class IMemoryHost
    {
    public:
        IMemoryHost();

        uint64_t alloc4KPages(uint64_t amount);
        uint64_t alloc4KPages(uint64_t amount, AddressSpace* addressSpace);
        void     free4KPages(void* address, uint64_t amount);
        void     countUsage();

        IMemAlloc*    getKernMemAlloc() const;
        IMemAlloc*    getPageMemAlloc() const;
        AddressSpace* getKernelAddressSpace() const;

    private:
        IMemAlloc*    m_kmalloc;
        IMemAlloc*    m_physAlloc;
        AddressSpace* m_addressSpaceKern;
        uint64_t      m_totalSize;
        uint64_t      m_usableSize;
    };
}