#pragma once

#include <init/bootinfo.h>
#include <mm/page.h>
#include <mm/mem.h>
#include <macros>

#define SPARSEMEM_LIMIT 256
#define PAGES_PER_SECTION 262144

namespace Memory::Model {
    extern mem_section_t sections[SPARSEMEM_LIMIT];

    struct Section {
        page_t *pages;
        uint8_t nid;
    } mem_section_t;

    inline mem_section_t *GetSection(uint64_t address) { 
        return &((*sections)[address >> SECTION_SHIFT]); 
    }

    inline page_t *GetPage(uint64_t address) { 
        mem_section_t *sect = &((*sections)[address >> SECTION_SHIFT]);
        uint32_t offset = (address >> PAGE_SHIFT) - sect->pageOffset;
        return &sect->pages[offset];
    }

    void MemblockPresent(int nid, uint64_t addrStart, uint64_t addrEnd);
    void MemblocksInit(bootmem_t *memdat);
    void MemsectsInit();
} // namespace Memory
