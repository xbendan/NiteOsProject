#pragma once

#include <Mem/Page.h>
#include <Utils/LinkedList.h>
#include <Utils/Spinlock.h>
#include <macros>
#ifdef ARCH_X86_64
    #include <Arch/x86_64/CPU.h>
#elif ARCH_AARCH64
    
#elif ARCH_RISCV

#endif

#define SLAB_MAX_BLOCK_ORDER        16
#define SLAB_MAX_STRUCT_ORDER       1
#define SLAB_MAX_SIZE               ARCH_PAGE_SIZE
#define MAX_NUMA_COUNT              4

uint8_t BLOCK_ORDER(size_t size);

using namespace Memory;

namespace Memory
{
    /**
     * @brief Manages the resource for specific CPU
     * 
     * This struct provides fast accessing for CPU
     * Check whether there is free objects in the cpu cache
     * before getting object from memory node.
     */
    typedef struct SlabCPUCache
    {
        /* 
         * Pointer to the next object, this is a double pointer
         * because the area this pointer point to is also a pointer
         * to the next object.
         */
        void **freelist;
        PageFrame *page;
        LinkedList<PageFrame> partial;
    } slab_cpu_t;

    typedef struct SlabMemoryNode
    {
        Spinlock lock;
        uint64_t nr_partial;
        LinkedList<PageFrame> partial;
    } slab_node_t;

    typedef struct SlabCache
    {
        /* The list head to connect different cache */
        // listhead_t lru;
        /* Name of the object this cache managed */
        const char *name;

        /* Indicate the cache for individual CPU core */
        slab_cpu_t cpu_slab[MAX_CPU_AMOUNT];
        uint64_t flags;
        int size;
        int object_size;
        int object_align;
        int offset;
        slab_node_t *node[MAX_NUMA_COUNT];
        uint64_t min_partial;
        int reserved;
    } slab_cache_t;

    class SlabAllocator
    {
    private:
        slab_cache_t *m_Caches[SLAB_MAX_BLOCK_ORDER];

    public:
        SlabAllocator();
        ~SlabAllocator();

        PageFrame *Request4KPage(slab_cache_t *cache, uint64_t *addrVirt = nullptr);
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
        uintptr_t Allocate(size_t size);
        void Free(uintptr_t address);
    };

    extern SlabAllocator *g_KernelAllocator;

    page_t *Request4KPageMapped(slab_cache_t *cache, uint64_t *addrVirt);
    page_t *Request4KPage(slab_cache_t *cache);
    void SetPageSlub(slab_cache_t *cache, page_t *page, uintptr_t virt);
    
    slab_cpu_t *SlubGetCPU(slab_cache_t *cache);
    slab_cache_t *SlubGetCache(size_t size);
    page_t *SlubGetPartial(slab_cache_t *cache);
    page_t SlabFillCpuSlot(slab_cpu_t *slab_cpu, page_t *page);
} // namespace Memory