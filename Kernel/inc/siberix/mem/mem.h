#pragma once

#include <siberix/mem/address-space.h>
#include <siberix/mem/page.h>
#include <siberix/svc/svc-host.h>
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

    class IPageAlloc : public IMemAlloc
    {
    public:
        virtual uint64_t allocatePhysMemory4K(uint64_t amount)      = 0;
        virtual Page4K*  allocatePhysMemory4KPages(uint64_t amount) = 0;
        virtual void     freePhysMemory4K(uint64_t address)         = 0;
        virtual void     freePhysMemory4KPages(Page4K* page)        = 0;

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
}