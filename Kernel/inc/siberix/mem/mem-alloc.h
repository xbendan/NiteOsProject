#pragma once

#include <siberix/mem/buddy.h>
#include <siberix/mem/mem.h>
#include <siberix/mem/page.h>
#include <siberix/mem/range.h>
#include <stdcxx/linked_list.h>
#include <stdcxx/list.h>
#include <stdcxx/string.h>
#include <stdcxx/type-traits.h>
#include <stdcxx/types.h>
#include <xtra-math/align.h>

namespace Kern::Mem {
    class BuddyAlloc : public IPageAlloc
    {
    public:
        BuddyAlloc();
        ~BuddyAlloc();

        uint64_t allocatePhysMemory4K(uint64_t amount) override;
        Page4K*  allocatePhysMemory4KPages(uint64_t amount) override;
        void     freePhysMemory4K(uint64_t address) override;
        void     freePhysMemory4KPages(Page4K* page) override;

    private:
        /*
            The page list contains pages of 2^order size. Like the pages in
            freePages[0] is 2^0 = 1 pages.
         */
        Page4KFifoQueue m_pageLists[BUDDY_BOUND_UPPER + 1];
    };

    class BlockAlloc : public IPageAlloc
    {
    public:
        BlockAlloc();
        ~BlockAlloc();

        uint64_t allocatePhysMemory4K(uint64_t amount) override;
        Page4K*  allocatePhysMemory4KPages(uint64_t amount) { return nullptr; }
        void     freePhysMemory4K(uint64_t address) override;
        void     freePhysMemory4KPages(Page4K* page) {}

    private:
        AddressRange m_pageRange[256];
    };
} // namespace siberix::mem