#include <mixins/meta/res.h>
#include <mixins/types.h>

#define PAGE_SIZE_4K 4096ULL
#define PAGE_SIZE_2M 2097152ULL
#define PAGE_SIZE_1G 1073741824ULL
#define PAGE_FLAG_FREE (1 << 0)
#define PAGE_FLAG_KMEM (1 << 1)
#define PAGE_FLAG_SWAP (1 << 2)
#define PAGE_FLAG_LOCK (1 << 3)
#define PAGE_FLAG_DIRTY (1 << 4)
#define PAGE_FLAG_SHARE (1 << 5)
#define PAGE_PART_SIZE (PAGE_SIZE_1G / 4)
#define PAGE_PART_COUNT (PAGE_PART_SIZE / PAGE_SIZE_4K)

namespace sbrx::mem {
    struct PageRec
    {
        PageRec* _next;
        PageRec* _previous;
        uint8_t  _order;
        uint8_t  _flags;
        struct /* slub */
        {
            struct
            {
                uint32_t _inuse : 16;
                uint32_t _objects : 15;
                uint32_t _frozen : 1;
            } __attribute__((packed));
            void** _freelist;
        };
        union
        {
            uint64_t _priv;
            PageRec* _pageHead;
            void*    _kmemPool;
        };
        uint64_t _address;

        bool isLocked() { return _flags & PAGE_FLAG_LOCK; }
        bool isDirty() { return _flags & PAGE_FLAG_DIRTY; }
        bool isShared() { return _flags & PAGE_FLAG_SHARE; }
        bool isFree() { return _flags & PAGE_FLAG_FREE; }
        bool isAllocTaken() { return _flags & PAGE_FLAG_KMEM; }

        uint64_t size() { return (1 << _order) * PAGE_SIZE_4K; }

        [[nodiscard]]
        PageRec* split();

        PageRec* merge(PageRec* page);
        PageRec* merge();
    };

    enum class MemErr
    {
        OutOfMemory,
        BadMemory,
        InvalidAddress,
    };

    enum class PageErr
    {
        OutOfMemory,
        InvalidPageSize,
        InvalidAddress,
        PageNotFound,
        PageNotFree,
    };
}