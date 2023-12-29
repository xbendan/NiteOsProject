#include <siberix/init/boot-info.h>
#include <siberix/init/init-comp.h>
#include <siberix/main.h>
#include <siberix/mem/mem.h>
#include <xtra-math/div.h>

#define PAGE_PART_SIZE (PAGE_SIZE_1G / 4)

namespace Kern::Mem {
    BlockAlloc _blockAlloc;
    BuddyAlloc _buddyAlloc;
    Page4K**   _pageSets;

    IMemoryHost::IMemoryHost()
      : m_physAlloc(&(_blockAlloc = BlockAlloc()))
      , m_addressSpaceKern(Init::setupVirtMemory())
    {
        Init::BootConfigTable::Mem& mem = Main::getBootConfig()->_mem;

        m_totalSize  = mem._totalSize;
        m_usableSize = mem._usableSize;

        uint64_t partition = PAGE_SIZE_1G / 4;
        uint64_t max       = alignUp(mem._maxSize, partition);
        _pageSets          = (Page4K**)alloc4KPages(
          divCeil(max / partition * sizeof(Page4K**), PAGE_SIZE_4K));

        for (int i = 0; i < max; i++) {
            _pageSets[i] = alloc4KPages(
              divCeil(partition / PAGE_SIZE_4K * sizeof(Page4K), PAGE_SIZE_4K));
        }
    }

    Page4K* pageOf(uint64_t address)
    {
        // May have null pointer exception
        return &_pageSets[address / PAGE_PART_SIZE][address % PAGE_PART_SIZE];
    }
}