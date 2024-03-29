#pragma once

#include <macros>
#include <Mem/Memory.h>
#include <Utils/LinkedList.h>
#include <Utils/Spinlock.h>

// #if (defined(__x86_64__))
#ifdef ARCH_X86_64
#include <Arch/x86_64/MMU.h>
#endif

#define PAGE_AMOUNT_PER_BLOCK 1024
#define PAGE_MAX_SIZE (ARCH_PAGE_SIZE * PAGE_AMOUNT_PER_BLOCK)
#define PAGE_MAX_ORDER 10
#define PAGE_MIN_ORDER 0
#define PFLAGS_FREE 1
#define PFLAGS_KMEM (1 << 1)
#define EQUALS_POWER_TWO(x) (!((x) & ((x) - 1)))

// 1   2   4   8
// 16  32  64  128
// 256 512 1024

static inline size_t ALIGN_PAGE(size_t x)
{
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x += 1;
}

static inline uint8_t PAGE_ORDER(size_t size)
{
    uint8_t order = PAGE_MAX_ORDER;
    size_t m_size = PAGE_MAX_SIZE / ARCH_PAGE_SIZE;
    while (m_size > size)
    {
        m_size /= 2;
        order--;
    }
    return order;
}

namespace Memory 
{
    typedef struct PageFrame
    {
        listhead_t lru;
        uint8_t order;
        uint8_t flags;
        struct {
            uint32_t slab_inuse: 16;
            uint32_t slab_objects: 15;
            uint32_t slab_frozen: 1;
        } __attribute__((packed));
        union {
            uint64_t priv;
            void *slab_cache;
            PageFrame *first;
        };
        void **freelist;
        Spinlock lock;
        uint64_t address;
    } page_t;

    struct BuddyZone {
        /**
         * This array contains the areas struct
         * The lowest is 0, equals to 4KiB (1 page)
         * The highest is 10, equals to 4MiB (1024 pages)
         */
        LinkedList<page_t> pageList[PAGE_MAX_ORDER + 1];
        uint64_t flags; 
        Spinlock lock;
    };

    #define ZONE_NORMAL 1
    // typedef enum BuddyZoneEnum {
    //     ZoneDMA = 0,
    //     ZoneNormal = 1,
    //     ZoneHighMem = 2
    // } zonetype_t;

    extern BuddyZone zones[3];

    void BuddyInit();
    page_t *AllocatePhysMemory4KOrdered(uint8_t order);
    page_t *AllocatePhysMemory4K(size_t amount);
    void FreePhysMemory4K(uintptr_t address);
    void FreePhysMemory4K(page_t *page);
    void MarkPagesUsed(uint64_t addrStart, uint64_t addrEnd);
    page_t* ExpandPage(page_t* page);
    page_t* CombinePage(page_t *page);
    void CombinePage(page_t *lpage, page_t *rpage);

    static inline bool IsPageAligned(page_t* page) { return !((page->address) % ((1 << page->order) * ARCH_PAGE_SIZE)); }

    uintptr_t KernelAllocate4KPages(size_t amount);
    uintptr_t KernelFree4KPages(uintptr_t address, size_t amount);
} // namespace Memory