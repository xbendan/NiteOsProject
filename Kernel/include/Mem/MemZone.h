#pragma once

#include <Init/BootInfo.h>
#include <Mem/Page.h>
#include <Mem/Memory.h>
#include <Utils/ArrayList.h>
#include <Utils/Range.h>

#include <macros>

#define SPARSEMEM_LIMIT 256
#define PAGES_PER_SECTION 262144

namespace Memory {
    struct Section {
        page_t *pages;
        uint8_t nid;
    };

    extern SizedArrayList<ValueRange, MEMORY_MAP_LIMIT> g_Blocks;
    extern SizedArrayList<Section, SPARSEMEM_LIMIT> g_Sections;

    inline Section *sectionOf(uint64_t address) { 
        return &(g_Sections[address >> SECTION_SHIFT]);
    }

    inline page_t *pageOf(uint64_t address) {
        unsigned sid = address >> SECTION_SHIFT;
        Section *sect = &(g_Sections[sid]);
        uint32_t offset = (address >> PAGE_SHIFT) - (sid * PAGES_PER_SECTION);
        return &(sect->pages[offset]);
    }
} // namespace Memory
