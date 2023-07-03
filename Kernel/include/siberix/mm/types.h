#include <common/typedefs.h>
#include <utils/linked_list.h>
#include <utils/spinlock.h>

enum PageBlockType { BlkTypeAvailable, BlkTypeKernel, BlkTypeReserved, BlkTypeBadram };

struct PageBlock {
    u64 start, end;
    PageBlockType type;

    PageBlock(u64 _start, u64 _end, PageBlockType _type) : start(_start), end(_end), type(_type) {}
};

#define PAGE_SIZE_4K 0x1000
#define PAGE_SIZE_2M 0x200000
#define PAGE_SIZE_1G 0x40000000

struct Pageframe {
    listhead_t lru;
    u8 order;
    u8 flags;
    struct {
        u32 slabInuse : 16;
        u32 slabObjects : 15;
        u32 slabFrozen : 1;
    } __attribute__((packed));
    union {
        u64 priv;
        void* slabCache;
        Pageframe* first;
    };
    void** freelist;
    spinlock_t lock;
    u64 address;
};

struct PageSection {
    u16 nid;
    u64* pages;
};

enum MemoryModelType { MmTypeFlat, MmTypeSparse };