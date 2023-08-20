#pragma once

#include <siberix/mm/types.h>
#include <utils/array.h>
#include <utils/linked_list.h>
#include <utils/spinlock.h>

#define SECTION_PAGE_SIZE 0x1000000
#define PAGES_PER_SECTION 262144
#define PAGES_PER_SET 1024
#define PAGE_MAX_SIZE (PAGE_SIZE_4K * PAGES_PER_SET)
#define PAGE_MAX_ORDER 10
#define PAGE_MIN_ORDER 0
#define PFLAGS_FREE (1 << 0)
#define PFLAGS_KMEM (1 << 1)

class SegAlloc : public PageAlloc {
public:
    SegAlloc();
    ~SegAlloc();

    Pageframe*                      allocatePhysMemory4K(u64 amount) override;
    void                            freePhysMemory4K(u64 address) override;
    void                            freePhysMemory4K(Pageframe* page) override;
    void                            addSegment(u64 start, u64 end, PageBlockType type);
    SizedArrayList<PageBlock, 256>* getSegments();

private:
    SizedArrayList<PageBlock, 256> segments;
};

class BuddyAlloc : public PageAlloc {
public:
    BuddyAlloc();
    ~BuddyAlloc();

    Pageframe* allocatePhysMemory4K(u64 amount) override;
    void       freePhysMemory4K(u64 address) override;
    void       freePhysMemory4K(Pageframe* page) override;
    void       markPagesUsed(u64 addressStart, u64 addressEnd);
    Pageframe* expand(Pageframe* page);
    Pageframe* combine(Pageframe* page);
    Pageframe* combine(Pageframe* lpage, Pageframe* rpage);

    static inline bool checkAlignment(Pageframe* page) {
        return !((page->address) % ((1 << page->order) * PAGE_SIZE_4K));
    }
    static inline u64 getPageAlignment(u64 x) {
        x--;
        x        |= x >> 1;
        x        |= x >> 2;
        x        |= x >> 4;
        x        |= x >> 8;
        x        |= x >> 16;
        return x += 1;
    }
    static inline u8 getPageOrder(u64 size) {
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
    LinkedList<Pageframe> pageList[PAGE_MAX_ORDER + 1];
    u64                   flags;
    spinlock_t            lock;
};

u64          page2pfn(Pageframe* page);
Pageframe*   pfn2page(u64 pfn);
Pageframe*   addr2page(u64 address);
PageSection* addr2sect(u64 address);