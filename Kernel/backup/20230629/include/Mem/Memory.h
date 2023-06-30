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
#define KiB(x) (x * 1024)
#define MiB(x) (x * 1048576)
#define GiB(x) (x * 1073741824)
#define TiB(x) (x * 1099511627776)

extern uint64_t KERNEL_START_ADDR;
extern uint64_t KERNEL_END_ADDR;

namespace Memory {
    enum MemoryStat
    {
        MemValTotal,
        MemValAvailable,
        MemValPresent,
        MemValSwapped,
        MemValInused,
        MemValStandby,
        MemValCached,
        MemValAllocated
    };

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