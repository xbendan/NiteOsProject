#pragma once

#include <siberix/mem/address-space.h>
#include <siberix/mem/kmem-alloc.h>
#include <siberix/mem/page.h>
#include <siberix/svc/svc-host.h>
#include <stdcxx/types.h>
#include <xtra-math/align.h>

namespace Kern {
    namespace Mem {
        uint64_t copyAsIoAddress(uint64_t address);

        class IMemAlloc
        {
        public:
            virtual uint64_t alloc(uint64_t size) = 0;
            virtual void     free(void* address)  = 0;
        };

        class IPageAlloc : virtual public IMemAlloc
        {
        public:
            virtual uint64_t allocatePhysMemory4K(uint64_t amount)      = 0;
            virtual Page4K*  allocatePhysMemory4KPages(uint64_t amount) = 0;
            virtual void     freePhysMemory4K(uint64_t address)         = 0;
            virtual void     freePhysMemory4KPages(Page4K* page)        = 0;

        private:
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

    namespace Svc {
        using namespace Mem;

        class MemSvcHost : Svc::ISvcHost
        {
        public:
            MemSvcHost()
              : ISvcHost("KERN.MEM", nullptr)
            {
            }

            uint64_t alloc4KPages(uint64_t amount);
            uint64_t alloc4KPages(uint64_t           amount,
                                  Mem::AddressSpace* addressSpace);
            void     free4KPages(void* address, uint64_t amount);

            IMemAlloc*    getKernMemAlloc() const;
            IMemAlloc*    getPageMemAlloc() const;
            AddressSpace* getKernelAddressSpace() const;

            // Std::String<Utf8>  getSvcName() override { return "kern.mem\0"; }
            // Std::String<Utf8>* getSvcDepends() override { return nullptr; }
            void onLoad() override;
            void onEnable() override;
            void onDisable() override;

        private:
            IMemAlloc*    m_kmalloc;
            IMemAlloc*    m_physAlloc;
            AddressSpace* m_addressSpaceKern;
            uint64_t      m_totalSize;
            uint64_t      m_usableSize;
        };
    }
}