#pragma once

#include <Macros>
#include <Memory/Memory.h>
#include <Utils/List.h>
#include <Utils/Spinlock.h>
#include <Utils/Range.h>

#define PAGE_MAX_SIZE (4 * 1024 * 1024)
#define PAGE_MAX_ORDER 10
#define PAGE_MIN_ORDER 0
#define PAGE_AMOUNT_PER_BLOCK 1024
#define EQUALS_POWER_TWO(x) (!((x) & ((x) - 1)))

namespace Memory
{
    /**
     * @brief Buddy page is the main unit of buddy system
     * It will be saved into the free area.
     * The page size must equals to 2^N
     */
    typedef struct PageData
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
            struct KMemoryCache *slabCache;
            void **freelist; 
        };
        Utils::Spinlock lock;
        uintptr_t addr;
    } pagedata_t;

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
        Utils::LinkedList<pagedata_t *> dataList;
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
        Utils::LinkedList<pagedata_t *> freelist[PAGE_MAX_ORDER + 1];
    };

    void LoadPageAllocator();
    pagedata_t* AllocatePages(uint8_t order);
    void FreePages(uintptr_t addr);
    void FreePages(pagedata_t* pf);
    void MarkPagesUsed(Range range);
    pagedata_t* GetPage(uintptr_t addr);
    pagedata_t* ExpandPage(pagedata_t* pf);
    pagedata_t* CombinePage(pagedata_t *pf);
    void CombinePages(pagedata_t *lpage, pagedata_t *rpage);
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

static inline bool IS_PAGE_ALIGNED(PageData *page)
{
    return !((page->addr) % ((1 << page->order) * ARCH_PAGE_SIZE));
}