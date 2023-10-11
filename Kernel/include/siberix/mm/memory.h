#pragma once

#include <siberix/mm/addrspace.h>
#include <siberix/mm/page.h>
#include <siberix/mm/types.h>

namespace siberix::mm {
    class MemoryAllocator
    {};
}

class MemoryServiceProvider
{
public:
    MemoryServiceProvider();
    ~MemoryServiceProvider();

    /**
     * @brief
     *
     * @param amount
     * @return u64
     */
    u64        alloc4KPages(u64 amount);
    /**
     * @brief
     *
     * @param amount
     * @param addressSpace
     * @return u64
     */
    u64        alloc4KPages(u64 amount, AddressSpace* addressSpace);
    /**
     * @brief
     *
     * @param amount
     * @param pageOut
     * @return u64
     */
    u64        alloc4KPages(u64 amount, PageFrame** pageOut);
    /**
     * @brief
     *
     * @param amount
     * @param addressSpace
     * @param pageOut
     * @return u64
     */
    u64        alloc4KPages(u64           amount,
                            AddressSpace* addressSpace,
                            PageFrame**   pageOut);
    /// @brief
    /// @param address
    void       free4KPages(u64 address, u64 amount);
    /// @brief
    /// @param amount
    /// @return
    PageFrame* allocPhysMemory4KPages(u64 amount);
    /// @brief
    /// @param amount
    /// @return
    u64        allocPhysMemory4K(u64 amount);
    /// @brief
    /// @param address
    void       freePhysMemory4KPages(u64 address);
    /// @brief
    /// @param page
    void       freePhysMemory4KPages(PageFrame* page);
    /// @brief
    /// @param amount
    /// @return
    u64        allocVirtMemory4KPages(u64 amount);
    /// @brief
    /// @param address
    void       freeVirtMemory4KPages(u64 address);
    /// @brief
    /// @param size
    /// @return
    u64        alloc(u64 size);
    /// @brief
    /// @param address
    void       free(u64 address);

    // u64        page2pfn(Pageframe& page) { return page.address /
    // PAGE_SIZE_4K; } Pageframe* pfn2page(u64 pfn) {
    //     u32 sectionId = pfn >> 18;
    //     u32 offset    = pfn - (sectionId * 262144);
    //     return
    //     reinterpret_cast<Pageframe*>(m_pageSections[sectionId].pages[offset]);
    // };
    // Pageframe*   addr2page(u64 address) { return pfn2page(address >> 12); }
    // PageSection* addr2sect(u64 address) { return &(m_pageSections[address >>
    // 30]); }

    u64  getTotalPages() { return this->totalPages; }
    u64  getAvailablePages() { return this->availablePages; }
    u64  getAllocatedPages() { return this->allocatedPages; }
    u64  getCachedPages() { return this->cachedPages; }
    u64  getSwappedPages() { return this->swappedPages; }
    void calculate();

    inline PageBlock&   getPageBlock(u8 index) { return m_pageBlocks[index]; }
    inline PageSection& getPageSect(u64 address)
    {
        return m_pageSections[address >> 30];
    }

private:
    u64 totalPages, availablePages, allocatedPages, cachedPages, swappedPages;
    PageAlloc*                       m_pageAlloc;
    MemoryAlloc*                     m_memoryAlloc;
    SizedArrayList<PageBlock, 256>   m_pageBlocks;
    SizedArrayList<PageSection, 256> m_pageSections;
};