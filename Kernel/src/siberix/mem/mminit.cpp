#include <siberix/init/boot-info.h>
#include <siberix/init/init-comp.h>
#include <siberix/main.h>
#include <siberix/mem/kmem-alloc.h>
#include <siberix/mem/mem-alloc.h>
#include <xtra-math/div.h>

namespace Kern::Svc {
    Page4K** _pageSets;

    void MemSvcHost::onLoad()
    {
        m_addressSpaceKern = Init::setupVirtMemory();

        static BlockAlloc _blockAlloc = BlockAlloc();
        m_physAlloc                   = &_blockAlloc;
        m_totalSize                   = Main::getBootConfig()->_mem._totalSize;
        m_usableSize                  = Main::getBootConfig()->_mem._usableSize;

        uint64_t partition = PAGE_SIZE_1G / 4;
        uint64_t max = alignUp(Main::getBootConfig()->_mem._maxSize, partition);
        _pageSets    = (Page4K**)alloc4KPages(
          divCeil(max / partition * sizeof(Page4K**), PAGE_SIZE_4K));

        for (int i = 0; i < max; i++) {
            _pageSets[i] = (Page4K*)alloc4KPages(
              divCeil(partition / PAGE_SIZE_4K * sizeof(Page4K), PAGE_SIZE_4K));
        }

        static BuddyAlloc _buddyAlloc = BuddyAlloc();
        m_physAlloc                   = &_buddyAlloc;

        static KernMemAlloc _kernMemAlloc = KernMemAlloc();
        m_kmalloc                         = &_kernMemAlloc;
    }

    Page4K* pageOf(uint64_t address)
    {
        // May have null pointer exception
        return &_pageSets[address / PAGE_PART_SIZE][address % PAGE_PART_SIZE];
    }
}
