#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mm/memory.h>
#include <utils/list.h>
#include <utils/spinlock.h>
#include <utils/range.h>
#include <mm/memory>
#include <utils/linkedlist>

#define PAGE_MAX_SIZE (4 * 1024 * 1024)
#define PAGE_MAX_ORDER 10
#define PAGE_MIN_ORDER 0
#define PAGE_AMOUNT_PER_BLOCK 1024
#define EQUALS_POWER_TWO(x) (!((x) & ((x) - 1)))

/**
 * @brief Buddy page is the main unit of buddy system
 * It will be saved into the free area.
 * The page size must equals to 2^N
 */
typedef struct Pageframe
{
    lklist_node_t listnode;
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
    spinlock_t lock;
    uintptr_t addr;
} pageframe_t;

/**
 * Each free area contains pages who have same size
 * which means that you cannot save a 16K page and a 256K
 * together.
 */
typedef struct PageframeList
{
    /**
     * This variable does not represents the whole list,
     * any valid node in the actual list could be saved here
     * but usually the first one
     */
    lklist_node_t handle;
    uint32_t count;
} pageframe_list_t;

typedef struct PageframeBlock
{   
    /* nodes available, one equals to 16 MiB */
    uint32_t count;
    uintptr_t addr;
    /**
     * This array contains the areas struct
     * The lowest is 0, equals to 4KiB (1 page)
     * The highest is 12, equals to 16MiB (4096 pages)
     */
    pageframe_list_t freelist[PAGE_MAX_ORDER + 1];
} pageframe_node_t;

size_t page_size_align(size_t size);
uint8_t page_size_order(size_t size);

void PM_LoadZoneRange(range_t range);
pageframe_t* PM_Allocate(size_t size);
pageframe_t* PM_AllocatePages(uint8_t order);
void PM_Free(uintptr_t addr);
void PM_FreePages(pageframe_t* pf);
void PM_MarkPagesUsed(range_t range);
pageframe_t* PM_GetPage(uintptr_t addr);
pageframe_t* PM_ExpandPage(pageframe_t* pf);
pageframe_t* PM_CombinePage(pageframe_t *pf);
void PM_CombineExistPages(pageframe_t *lpage, pageframe_t *rpage);


