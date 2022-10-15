#pragma once

#include <Init/BootInfo.h>
#include <Macros>
#define ARCH_PAGE_SIZE (4096)
#define MEMORY_MAP_LIMIT 64

extern uint64_t KERNEL_START_ADDR;
extern uint64_t KERNEL_END_ADDR;

namespace Memory
{
    extern MemoryStatistic memStats;

    enum MemoryMapEntryType
    {
        MemoryMapEntryTypeAvailable = 0,
        MemoryMapEntryTypeReserved = 1,
        MemoryMapEntryTypeAcpiReclaimable = 2,
        MemoryMapEntryTypeNvs = 3,
        MemoryMapEntryTypeBadRam = 4,
        MemoryMapEntryTypeKernel = 5
    };

    struct MemoryMapEntry
    {
        uint64_t m_AddrStart, m_AddrEnd;
        enum MemoryMapEntryType m_Type;
    };

    enum MemoryStatisticKey
    {
        MemValueTotal = 0,
        MemValueAvaliable = 1,
        MemValuePresent = 2,
        MemValueSwapped = 3,
        MemValueInused = 4,
        MemValueStandby = 5,
        MemValueCached = 6,
        MemValueAllocated = 7
    };

    struct MemoryStatistic
    {
        uint64_t total;
        uint64_t available;
        uint64_t present;
        uint64_t swapped;
        uint64_t inuse;
        uint64_t standby;
        uint64_t cached;
        uint64_t allocated;
    };

    void Initialize();
} // namespace Memory

//void mem_value_commit(enum mem_stats_item item_num, int64_t change);