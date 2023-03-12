#pragma once

#include <init/bootinfo.h>
#include <Mem/Page.h>
#include <Mem/Memory.h>
#include <macros>

#define SPARSEMEM_LIMIT 256
#define PAGES_PER_SECTION 262144

namespace Memory::Model {
    typedef struct MemorySection {
        page_t *pages;
        uint8_t nid;
    } mem_section_t;

    typedef struct MemoryBlock {
        uint64_t m_AddrStart, m_AddrEnd;
    } mem_block_t;

    extern mem_block_t memblocks[MEMORY_MAP_LIMIT];
    extern mem_section_t sections[SPARSEMEM_LIMIT];

    inline mem_section_t *GetSection(uint64_t address) { 
        return &(sections[address >> SECTION_SHIFT]);
    }

    inline page_t *GetPage(uint64_t address) {
        unsigned sid = address >> SECTION_SHIFT;
        mem_section_t *sect = &(sections[sid]);
        uint32_t offset = (address >> PAGE_SHIFT) - (sid * PAGES_PER_SECTION);
        return &(sect->pages[offset]);
    }

    void *MemblockAllocate(size_t amount);
    void MemblocksInit(bootmem_t *memdat);
    void MemsectPresent(int nid, uint64_t pfnStart, uint64_t pfnEnd);
    void MemsectsInit();
} // namespace Memory
