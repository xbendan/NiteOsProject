#include <common/logger.h>
#include <common/string.h>
#include <siberix/core/runtimes.h>
#include <siberix/mm/addrspace.h>
#include <siberix/mm/memory.h>
#include <siberix/mm/slab.h>
#include <siberix/proc/sched.h>
#include <utils/alignment.h>

SegAlloc*      _segAlloc;
BuddyAlloc*    _buddyAlloc;
SlabAlloc*     _slabAlloc;
extern Process kernelProcess;

#include <arch/x86_64/paging.h>

MemoryServiceProvider::MemoryServiceProvider()
{
    BootConfig& bootConfig = siberix()->getBootConfig();
    Logger::getAnonymousLogger().info(
      "[%u] bytes memory detected.\n[%u] bytes usable.\nMax memory address is "
      "at [0x%x].\n",
      bootConfig.memory.totalSize,
      bootConfig.memory.usableSize,
      bootConfig.memory.maxSize);

    PageBlock* pageBlock;
    for (int i = 0; i < 256; i++) {
        pageBlock = &(bootConfig.memory.ranges[i]);

        alignUpRef(pageBlock->m_start, PAGE_SIZE_4K);
        alignDownRef(pageBlock->m_end, PAGE_SIZE_4K);
        if (pageBlock->m_end == 0) {
            break;
        }
        if ((pageBlock->m_end - pageBlock->m_start) < PAGE_SIZE_4K) {
            continue;
        }

        Logger::getAnonymousLogger().info(
          "Page block imported. (Start[0x%x],End[0x%x],Type[%u])\n",
          pageBlock->m_start,
          pageBlock->m_end,
          pageBlock->m_type);

        m_pageBlocks.add(pageBlock);
    }

    static SegAlloc segAlloc = SegAlloc(m_pageBlocks);
    this->m_pageAlloc = _segAlloc = &segAlloc;

    AddressSpace* kernelAddressSpace =
      siberix()->getKernelProcess()->getAddressSpace();
    u64 address = 0;
    for (int i = 0; i < 256 && m_pageBlocks[i].m_end; i++) {
        pageBlock = &m_pageBlocks[i];
        if (pageBlock->m_type != PageBlockType::Available) {
            continue;
        }

        address = pageBlock->m_start;
        while (address < pageBlock->m_end) {
            PageSection& pageSect = m_pageSections[address >> 30];
            if (!pageSect.pages) {
                u64 virt =
                      kernelAddressSpace->allocate4KPages(SECTION_PAGE_USE),
                    phys       = allocPhysMemory4K(SECTION_PAGE_USE);
                pageSect.pages = reinterpret_cast<PageFrame*>(virt);
                kernelAddressSpace->map(phys, virt, SECTION_PAGE_USE);
            }
            address += PAGE_SIZE_1G;
        }
    }

    static BuddyAlloc buddyAlloc = BuddyAlloc(m_pageSections, m_pageBlocks);
    this->m_pageAlloc = _buddyAlloc = &buddyAlloc;

    static SlabAlloc slabAlloc = SlabAlloc(m_pageAlloc);
    this->m_memoryAlloc = _slabAlloc = &slabAlloc;

    Logger::getAnonymousLogger().success("Initialized memory allocation.\n");
}

MemoryServiceProvider::~MemoryServiceProvider() {}

u64
MemoryServiceProvider::alloc4KPages(u64 amount)
{
    return alloc4KPages(amount, thisProcess()->getAddressSpace(), nullptr);
}

u64
MemoryServiceProvider::alloc4KPages(u64 amount, AddressSpace* addressSpace)
{
    return alloc4KPages(amount, addressSpace, nullptr);
}

u64
MemoryServiceProvider::alloc4KPages(u64 amount, PageFrame** out_page)
{
    return alloc4KPages(amount, thisProcess()->getAddressSpace(), out_page);
}

u64
MemoryServiceProvider::alloc4KPages(u64           amount,
                                    AddressSpace* addressSpace,
                                    PageFrame**   pageOut)
{
    PageFrame* page = allocPhysMemory4KPages(amount);
    u64 phys = page->address, virt = addressSpace->allocate4KPages(amount);

    /*
        Check whether phys and virt contains valid address
        If not, consider page swapping or terminate the process
     */
    if (!phys) {
        Logger::getLogger("mem_alloc")
          .warn("This machine has already ran out of memory. Try swap pages\n");

        return 0;
    }
    if (!virt) {
        Logger::getLogger("mem_alloc")
          .warn("This process ran out of its address space. Consider terminate "
                "it.\n");
        return 0;
    }

    // Assign the out page pointer if it exists
    if (pageOut != nullptr) {
        *pageOut = page;
    }

    addressSpace->map(phys, virt, amount);
    return virt;
}

void
MemoryServiceProvider::free4KPages(u64 address, u64 amount)
{
}

PageFrame*
MemoryServiceProvider::allocPhysMemory4KPages(u64 amount)
{
    return m_pageAlloc->allocatePhysMemory4KPages(amount);
}

u64
MemoryServiceProvider::allocPhysMemory4K(u64 amount)
{
    return m_pageAlloc->allocatePhysMemory4K(amount);
}

void
MemoryServiceProvider::freePhysMemory4KPages(u64 address)
{
    m_pageAlloc->freePhysMemory4K(addr2page(address));
}

void
MemoryServiceProvider::freePhysMemory4KPages(PageFrame* page)
{
    m_pageAlloc->freePhysMemory4K(page);
}

u64
MemoryServiceProvider::allocVirtMemory4KPages(u64 amount)
{
    return thisProcess()->getAddressSpace()->allocate4KPages(amount);
}

void
MemoryServiceProvider::freeVirtMemory4KPages(u64 address)
{
}

u64
MemoryServiceProvider::alloc(u64 size)
{
    return m_memoryAlloc->alloc(size);
}

void
MemoryServiceProvider::free(u64 address)
{
    m_memoryAlloc->free(address);
}

void
MemoryServiceProvider::calculate()
{
}