#include <common/typedefs.h>
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

struct Page {
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
        Page *first;
    };
    void **freelist;
    spinlock_t lock;
    u64 address;
}