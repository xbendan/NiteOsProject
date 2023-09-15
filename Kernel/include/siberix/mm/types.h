#pragma once

#include <common/typedefs.h>
#include <utils/linked_list.h>
#include <utils/spinlock.h>

enum PageBlockType
{
    Available,
    KernelOrModule,
    Reserved,
    Badram,
    Reclaimable
};

struct PageBlock
{
    u64           m_start, m_end;
    PageBlockType m_type;

    PageBlock()
      : m_start(0)
      , m_end(0)
      , m_type(PageBlockType::Reserved)
    {
    }

    PageBlock(PageBlock& pageBlock)
      : m_start(pageBlock.m_start)
      , m_end(pageBlock.m_end)
      , m_type(pageBlock.m_type)
    {
    }

    PageBlock(u64 _start, u64 _end, PageBlockType _type)
      : m_start(_start)
      , m_end(_end)
      , m_type(_type)
    {
    }
};

#define PAGE_SIZE_4K 0x1000UL
#define PAGE_SIZE_2M 0x200000UL
#define PAGE_SIZE_1G 0x40000000UL

struct PageFrame
{
    ListHead lru;
    u8       order;
    u8       flags;
    struct
    {
        u32 slabInuse : 16;
        u32 slabObjects : 15;
        u32 slabFrozen : 1;
    } __attribute__((packed));
    union
    {
        u64        priv;
        void*      slabCache;
        PageFrame* first;
    };
    void**     freelist;
    spinlock_t lock;
    u64        address;
};

struct PageSection
{
    u16        nid;
    PageFrame* pages;
};

class MemoryAlloc
{
public:
    virtual u64  alloc(u64 size)   = 0;
    virtual void free(u64 address) = 0;
};

class PageAlloc
{
public:
    virtual PageFrame* allocatePhysMemory4KPages(u64 amount) = 0;
    virtual u64        allocatePhysMemory4K(u64 amount)      = 0;
    virtual void       freePhysMemory4K(u64 address)         = 0;
    virtual void       freePhysMemory4K(PageFrame* page)     = 0;
};