#include <siberix/mm/page.h>

#define SLAB_MAX_BLOCK_ORDER 16

namespace Memory {
    struct SlabCpuCache {
        /*
         * Pointer to the next object, this is a double pointer
         * because the area this pointer point to is also a pointer
         * to the next object.
         */
        void**                freelist;
        Pageframe*            page;
        LinkedList<Pageframe> partial;
    };

    struct SlabNode {
        Spinlock              lock;
        u64                   nr_partial;
        LinkedList<Pageframe> partial;
    };

    class SlabCache {
    public:
        SlabCache(u64 size, u64 flags);
        ~SlabCache();

        Pageframe* request4KPage(u64* addrVirt);

        /* The list head to connect different cache */
        // listhead_t lru;
        /* Name of the object this cache managed */
        const char* name;

        /* Indicate the cache for individual CPU core */
        SlabCpuCache cpus[256];
        u64          flags;
        u32          size;
        u32          objectSize;
        u32          objectAlignment;
        u32          offset;
        SlabNode*    nodes[4];
        u64          min_partial;
        u32          reserved;
    };

    class SlabAlloc : public MemoryAlloc {
    public:
        SlabAlloc();
        ~SlabAlloc();

        /**
         * @brief Find a free object
         *
         * The sequence of allocating kernel memory:
         * 1. Use [freelist] in cpu slab
         *      Yes -> Make [freelist] point to the next object
         *      No  -> Check whether there is a free object in current page
         *          Yes -> If page exists, use [freelist] in page
         *          No  -> (2)
         * 2. Use pointer from partial list
         *      Yes -> Assign [freelist] and [page] from partial list
         *      No  -> (3)
         * 3. Try to allocate from partial list from cache node
         *      Yes -> Assign [freelist] and [page] in cpu slab and move 1
         *             page to the partial list of cpu slab
         *      No  -> (4)
         * 4. Allocate from buddy allocator
         *      Yes -> Assign all values
         *      No  -> PANIC! Out of memory.
         * @param cache
         * @return uintptr_t
         */
        u64  alloc(u64 size) override;
        void free(u64 address) override;

        SlabCache* getCache(u32 size);

        SlabCache* caches[SLAB_MAX_BLOCK_ORDER];
    };
}  // namespace Memory
