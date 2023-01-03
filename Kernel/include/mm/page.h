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
#define EQUALS_POWER_TWO(x) (!((x) & ((x) - 1)))

namespace Memory
{
    /**
     * @brief Buddy page is the main unit of buddy system
     * It will be saved into the free area.
     * The page size must equals to 2^N
     */
    typedef struct PageFrame
    {
        Utils::ListHead listnode;
        struct
        {
            uint8_t order: 4;
            bool free: 1;
            bool kmem: 1;
            uint16_t __ign: 10;
        } __attribute__((packed));
        struct
        {
            /*
            union
            {
                lklist_node_t slablist;
                struct
                {
                    void *next;
                    int pages;
                    int pobjects;
                };
            };
            */
            struct
            {
                uint32_t slab_inuse: 16;
                uint32_t slab_objects: 15;
                uint32_t slab_frozen: 1;
            } __attribute__((packed));
            struct SlabCache *slabCache;
            void **freelist; 
        };
        Utils::Spinlock lock;
        uintptr_t addr;
    } page_t;

    /**
     * Each free area contains pages who have same size
     * which means that you cannot save a 16K page and a 256K
     * together.
     */
    struct PageList
    {
        /**
         * This variable does not represents the whole list,
         * any valid node in the actual list could be saved here
         * but usually the first one
         */
        Utils::LinkedList<PageFrame *> dataList;
        uint32_t count;
    };

    struct PageBlock
    {   
        /* nodes available, one equals to 16 MiB */
        uint32_t count;
        uintptr_t addr;
        /**
         * This array contains the areas struct
         * The lowest is 0, equals to 4KiB (1 page)
         * The highest is 12, equals to 16MiB (4096 pages)
         */
        Utils::LinkedList<PageFrame *> freelist[PAGE_MAX_ORDER + 1];
    };

    void LoadPageAllocator();
    page_t* AllocatePages(uint8_t order);
    void FreePages(uintptr_t addr);
    void FreePages(page_t* pf);
    void MarkPagesUsed(Range range);
    page_t* GetPage(uintptr_t addr);
    page_t* ExpandPage(page_t* pf);
    page_t* CombinePage(page_t *pf);
    void CombinePage(page_t *lpage, page_t *rpage);

    static inline bool IsPageAligned(page_t* page) { return !((page->addr) % ((1 << page->order) * ARCH_PAGE_SIZE)); }
} // namespace Memory

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
        LinkedList<page_t> pageList[PAGE_MAX_ORDER + 1];
        uint64_t flags; 
    } buddyzone_t;

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