#pragma once

#include <siberix/mm/types.h>
#include <utils/array.h>
#include <utils/linked_list.h>
#include <utils/spinlock.h>

#define SECTION_PAGE_SIZE 0x1000000
#define SECTION_PAGE_USE 0x1000
#define PAGES_PER_SECTION 0x40000
#define PAGES_PER_SET 1024
#define PAGE_MAX_SIZE (PAGE_SIZE_4K * PAGES_PER_SET)
#define PAGE_MAX_ORDER 10
#define PAGE_MIN_ORDER 0
#define PFLAGS_FREE (1 << 0)
#define PFLAGS_KMEM (1 << 1)

class SegAlloc : public PageAlloc
{
public:
    SegAlloc(SizedArrayList<PageBlock, 256>& blockRefs);
    ~SegAlloc();

    PageFrame* allocatePhysMemory4KPages(u64 amount) override;
    u64        allocatePhysMemory4K(u64 amount) override;
    void       freePhysMemory4K(u64 address) override;
    void       freePhysMemory4K(PageFrame* page) override;

private:
    SizedArrayList<PageBlock, 256>& m_blockRefs;
};

class BuddyAlloc : public PageAlloc
{
public:
    BuddyAlloc(SizedArrayList<PageSection, 256>& sectRefs,
               SizedArrayList<PageBlock, 256>&   blockRefs);
    ~BuddyAlloc();

    PageFrame* allocatePhysMemory4KPages(u64 amount) override;
    u64        allocatePhysMemory4K(u64 amount) override;
    void       freePhysMemory4K(u64 address) override;
    void       freePhysMemory4K(PageFrame* page) override;
    void       markPagesUsed(u64 addressStart, u64 addressEnd);
    PageFrame* expand(PageFrame* page);
    PageFrame* combine(PageFrame* page);
    PageFrame* combine(PageFrame* lpage, PageFrame* rpage);

    static inline bool checkAlignment(PageFrame* page)
    {
        return !((page->address) % ((1 << page->order) * PAGE_SIZE_4K));
    }
    static inline u64 getPageAlignment(u64 x)
    {
        x--;
        x        |= x >> 1;
        x        |= x >> 2;
        x        |= x >> 4;
        x        |= x >> 8;
        x        |= x >> 16;
        return x += 1;
    }
    static inline u8 getPageOrder(u64 size)
    {
        u8  order = PAGE_MAX_ORDER;
        u64 _size = 1024;
        while (_size > size) {
            _size /= 2;
            order--;
        }
        return order;
    }

private:
    /**
     * This array contains the areas struct
     * The lowest is 0, equals to 4KiB (1 page)
     * The highest is 10, equals to 4MiB (1024 pages)
     */
    LinkedList<PageFrame> pageList[PAGE_MAX_ORDER + 1];
    u64                   flags;
    spinlock_t            lock;
};

u64
page2pfn(PageFrame* page);
PageFrame*
pfn2page(u64 pfn);
PageFrame*
addr2page(u64 address);
PageSection*
addr2sect(u64 address);