#pragma once

#include <macros>
#define ARCH_PAGE_SIZE (4096)
#define MEMORY_MAP_LIMIT 64
#define SECTION_SHIFT 30
#define PAGE_SHIFT 12
#define PAGE_SECTION_SHIFT (SECTION_SHIFT - PAGE_SHIFT)
#define MEM_MAXSIZE 0x100000000000ULL
#define SIZE_1K (1 << 10)
#define SIZE_1M (1 << 20)
#define SIZE_1G (1 << 30)
#define SIZE_1T (1 << 40)
#define MEMVAL_TOTAL 0
#define MEMVAL_AVAILABLE 1
#define MEMVAL_PRESENT 2
#define MEMVAL_SWAPPED 3
#define MEMVAL_INUSED 4
#define MEMVAL_STANDBY 5
#define MEMVAL_CACHED 6
#define MEMVAL_ALLOCATED 7

extern uint64_t KERNEL_START_ADDR;
extern uint64_t KERNEL_END_ADDR;

namespace Memory {
    enum MemoryMapEntryType {
        MemoryMapEntryTypeAvailable = 0,
        MemoryMapEntryTypeReserved = 1,
        MemoryMapEntryTypeAcpiReclaimable = 2,
        MemoryMapEntryTypeNvs = 3,
        MemoryMapEntryTypeBadRam = 4,
        MemoryMapEntryTypeKernel = 5
    };

    typedef struct MemoryMapEntry {
        uint64_t m_AddrStart, m_AddrEnd;
        enum MemoryMapEntryType m_Type;
    } memmap_entry_t;

    void Initialize();
} // namespace Memory

//void mem_value_commit(enum mem_stats_item item_num, int64_t change);