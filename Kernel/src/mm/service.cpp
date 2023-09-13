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

        alignUpRef(pageBlock->start, PAGE_SIZE_4K);
        alignDownRef(pageBlock->end, PAGE_SIZE_4K);
        if (pageBlock->end == 0) {
            break;
        }
        if ((pageBlock->end - pageBlock->start) < PAGE_SIZE_4K) {
            continue;
        }

        Logger::getAnonymousLogger().info(
          "A page block imported. (Start[0x%x],End[0x%x],Type[%u])\n",
          pageBlock->start,
          pageBlock->end,
          pageBlock->type);

        m_pageBlocks.add(*pageBlock);
    }

    static SegAlloc segAlloc = SegAlloc();
    this->m_pageAlloc = _segAlloc = &segAlloc;

    AddressSpace* kernelAddressSpace =
      siberix()->getKernelProcess()->getAddressSpace();
    u64 address = 0;
    for (int i = 0; i < 256 && m_pageBlocks[i].end; i++) {
        pageBlock = &(m_pageBlocks[i]);
        if (pageBlock->type != PageBlockType::Available) {
            continue;
        }

        address = pageBlock->start;
        while (address < pageBlock->end) {
            PageSection& pageSect = m_pageSections[address >> 30];
            if (!pageSect.pages) {
                u64 virt =
                      kernelAddressSpace->allocate4KPages(SECTION_PAGE_USE),
                    phys       = allocPhysMemory4K(SECTION_PAGE_USE);
                pageSect.pages = reinterpret_cast<u64*>(virt);
                kernelAddressSpace->map(phys, virt, SECTION_PAGE_USE);
            }
            address += PAGE_SIZE_1G;
        }
    }

    for (;;)
        asm("hlt");

    static BuddyAlloc buddyAlloc = BuddyAlloc();
    this->m_pageAlloc = _buddyAlloc = &buddyAlloc;

    static SlabAlloc slabAlloc = SlabAlloc();
    this->m_memoryAlloc = _slabAlloc = &slabAlloc;

    Logger::getAnonymousLogger().info("Initialized memory allocation.\n");
}

MemoryServiceProvider::~MemoryServiceProvider() {}

u64
MemoryServiceProvider::alloc4KPages(u64 amount)
{
    return alloc4KPages(amount, nullptr);
}

u64
MemoryServiceProvider::alloc4KPages(u64 amount, Pageframe** _pointer)
{
    Pageframe* page = allocPhysMemory4KPages(amount);

    if (_pointer != nullptr)
        *_pointer = page;
    u64 phys = page->address;
    u64 virt = allocVirtMemory4KPages(amount);
    if (!(phys && virt)) {
        return 0;
    }
    thisProcess()->getAddressSpace()->map(virt, phys, amount);
    return virt;
}

void
MemoryServiceProvider::free4KPages(u64 address, u64 amount)
{
}

Pageframe*
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
MemoryServiceProvider::freePhysMemory4KPages(Pageframe* page)
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