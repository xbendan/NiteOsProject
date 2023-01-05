#pragma once

#include <macros>
#include <mm/mem.h>
#include <utils/list.h>
#include <utils/spinlock.h>
#include <utils/range.h>

#ifdef ARCH_X86_64
#include <Arch/x86_64/MMU.h>
#endif

#define PAGE_AMOUNT_PER_BLOCK 1024
#define PAGE_MAX_SIZE (ARCH_PAGE_SIZE * PAGE_AMOUNT_PER_BLOCK)
#define PAGE_MAX_ORDER 10
#define PAGE_MIN_ORDER 0
#define PFLAGS_ORDER 0x0F
#define PFLAGS_FREE 0x10
#define EQUALS_POWER_TWO(x) (!((x) & ((x) - 1)))

using namespace Utils;

namespace Memory 
{
    typedef struct PageFrame {
        listhead_t lru;
        uint16_t flags;
        struct {
            uint32_t slab_inuse: 16;
            uint32_t slab_objects: 15;
            uint32_t slab_frozen: 1;
        } __attribute__((packed));
        union {
            uint64_t priv;
            slab_cache_t *slab;
            page_t *first;
        };
        void **freelist;
        spinlock_t lock;
        uint64_t addr;
    } page_t;

    typedef struct BuddyZone {
        /**
         * This array contains the areas struct
         * The lowest is 0, equals to 4KiB (1 page)
         * The highest is 10, equals to 4MiB (1024 pages)
         */
        LinkedList<page_t> pageList[PAGE_MAX_ORDER + 1];
        uint64_t flags; 
        spinlock_t lock;
    } buddyzone_t;

    typedef enum BuddyZoneEnum {
        ZoneDMA = 0,
        ZoneNormal = 1,
        ZoneHighMem = 2
    } zonetype_t;

    extern BuddyZone *zones;

    void BuddyInit();
    page_t *Allocate4KPages(size_t amount);
    void Free4KPages(uintptr_t address);
    void Free4KPages(page_t *page);
    void MarkPagesUsed(Range range);
    page_t* ExpandPage(page_t* pf);
    page_t* CombinePage(page_t *pf);
    void CombinePage(page_t *lpage, page_t *rpage);

    static inline bool IsPageAligned(page_t* page) { return !((page->addr) % ((1 << page->order) * ARCH_PAGE_SIZE));
} // namespace Memory


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