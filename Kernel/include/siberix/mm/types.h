#include <common/typedefs.h>
#include <utils/linked_list.h>
#include <utils/spinlock.h>

enum AddressSegmentType
{
    AST_AVAILABLE,
    AST_KERNEL,
    AST_RESERVED,
    AST_BADRAM
};

struct AddressSegment
{
    u64 start, end;
    AddressSegmentType type;

    AddressSegment(u64 start, u64 end, AddressSegmentType type)
      : start(start),
        end(end),
        type(type) { }
};

#define PAGE_SIZE_4K 0x1000
#define PAGE_SIZE_2M 0x1000000
#define PAGE_SIZE_1G 0x1000000000

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
        void *slabCache;
        Pageframe *first;
    };
    void **freelist;
    spinlock_t lock;
    u64 address;
};