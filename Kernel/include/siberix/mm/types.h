#pragma once

#include <common/typedefs.h>
#include <utils/linked_list.h>
#include <utils/spinlock.h>

enum PageBlockType {
    BlkTypeAvailable,
    BlkTypeKernel,
    BlkTypeReserved,
    BlkTypeBadram
};

struct PageBlock {
    u64           start, end;
    PageBlockType type;

    PageBlock()
        : start(0),
          end(0),
          type(BlkTypeReserved) {}

    PageBlock(u64 _start, u64 _end, PageBlockType _type)
        : start(_start),
          end(_end),
          type(_type) {}
};

#define PAGE_SIZE_4K 0x1000
#define PAGE_SIZE_2M 0x200000
#define PAGE_SIZE_1G 0x40000000

struct Pageframe {
    ListHead lru;
    u8       order;
    u8       flags;
    struct {
        u32 slabInuse : 16;
        u32 slabObjects : 15;
        u32 slabFrozen : 1;
    } __attribute__((packed));
    union {
        u64        priv;
        void*      slabCache;
        Pageframe* first;
    };
    void**     freelist;
    spinlock_t lock;
    u64        address;
};

struct PageSection {
    u16  nid;
    u64* pages;
};

class MemoryAlloc {
public:
    virtual u64  alloc(u64 size)   = 0;
    virtual void free(u64 address) = 0;
};

class PageAlloc {
public:
    virtual Pageframe* allocatePhysMemory4K(u64 amount)  = 0;
    virtual void       freePhysMemory4K(u64 address)     = 0;
    virtual void       freePhysMemory4K(Pageframe* page) = 0;
};