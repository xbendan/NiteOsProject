#include <Memory/Slub.h>
#include <Process/Process.h>
#include <Process/Scheduler.h>
#include <utils/list.h>
#include <Kernel>

#ifdef ARCH_X86_64
#include <Arch/x86_64/MMU.h>
#include <Arch/x86_64/CPU.h>
#endif

namespace Memory
{
    const uint16_t blockSize[] = {
        8,      16,     32,     48,     64,     96,     128,    192, 
        256,    512,    768,    1024,   1536,   2048,   4096,   8192,
        sizeof(Proc::Thread)
    };
    static SlabCache *g_cachePointers[SLAB_MAX_BLOCK_ORDER];
    static Utils::LinkedList<SlabCache> g_cacheList;

    void kmem_set_cache(struct SlabCache *cache, int order, uint64_t flags)
    {
        if(cache == NULL)
        {
            CallPanic("Invalid address");
            return;
        }

        int size = blockSize[size];

        cache->size = size;
        cache->flags = flags;
        
        if(ARCH_PAGE_SIZE % size != 0)
        {
            int amount = ARCH_PAGE_SIZE / size;
            cache->reserved = ARCH_PAGE_SIZE - (amount * size);
        }

        for(size_t idx = 0; idx < MAX_CPU_AMOUNT; idx++)
        {
            cache->cpu_slab[idx].freelist = NULL;
        }

        for(size_t idx = 0; idx < MAX_NUMA_COUNT; idx++)
            cache->node[idx] = (struct SlabMemoryNode *)(((uint64_t) cache) + (idx * sizeof(struct SlabMemoryNode)));

        g_cacheList.Add(&cache->listnode);
        g_cachePointers[order] = cache;
    }

    void kmem_set_page(struct SlabCache *cache, PageFrame *page, uintptr_t virt)
    {
        page->kmem = true;
        page->slabCache = cache;
        page->freelist = (void **) virt;

        page->slab_objects = ((ARCH_PAGE_SIZE - cache->reserved) / cache->size);
        page->slab_inuse = 0;

        for(size_t idx = 0; idx < page->slab_objects - 1; idx++)
            *((uint64_t *)(virt + (idx * cache->size))) = (uint64_t)(virt + ((idx + 1) * cache->size));
    }

    /**
     *
     * 
     */
    PageFrame *SlabCache_alloc(struct SlabCache *cache)
    {
        uintptr_t virt = kallocpg(1);
        if(virt == 0x0)
            CallPanic("");
        else
        {
            PageFrame *page = GetPage(ConvertVirtToPhys(Memory::GetKernelPages(), virt));
            kmem_set_page(cache, page, virt);
            return page;
        }
    }

    struct SlabCpuCache *kmem_get_cpu(struct SlabCache *cache)
    {
        return &cache->cpu_slab[CPU_CORE_ID];
    }

    struct SlabCache *kmem_find_cache(size_t size)
    {
        struct SlabCache *cache;

        if(size > SLAB_MAX_SIZE)
            return NULL;

        for(int order = 0; order < 16; order++)
        {
            if(blockSize[order] > size)
            {
                cache = g_cachePointers[order];
                break;
            }
        }

        return cache;
    }

    /**
     * @brief 
     * 
     * @param cache 
     * @return PageFrame* 
     */
    PageFrame *kmem_get_partial(struct SlabCache *cache)
    {
        struct SlabMemoryNode *node = cache->node[0];
        struct PageFrame *page;

        node->lock.Acquire();

        page = node->partial.Count() ?
                ((PageFrame *) node->partial.Extract()) :
                SlabCache_alloc(cache);
        
        /*
        if(node->nr_partial == 0)
            page = SlabCache_alloc(cache);
        else
        {
            PageFrame *page = (PageFrame *) node->partial.Extract();
            node->nr_partial--;
        }
        */

        node->lock.Release();
        return page;
    }

    /**
     * @brief 
     * 
     * @param slabCpu 
     * @param page 
     */
    void kmem_fill_cpu_slab(struct SlabCpuCache *slabCpu, PageFrame *page)
    {
        //LinkedListRemove(&page->listnode);

        slabCpu->freelist = page->freelist;
        slabCpu->page = page;
        
        page->slab_frozen = true;
    }

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
    uintptr_t allocobj(SlabCache *cache)
    {
        if (cache == NULL)
            CallPanic("Null Pointer for cache while allocating kernel memory.");
        
        SlabCpuCache *slabCpu = kmem_get_cpu(cache);
        Memory::PageFrame *page = slabCpu->page;
        void **ptr;

    FASTEST_PATH:
        /*
        * Try to use the object in cpu slab first
        */
        ptr = slabCpu->freelist;

        /* (1) Fastest Path */
        if(ptr != NULL) {
            /* Assign freelist point to the next object (might be NULL) */
            page->freelist = slabCpu->freelist = (void **) *ptr;

            /* Check whether the objects in this page is running out */
            if(++page->slab_inuse == page->slab_objects) {
                kmem_fill_cpu_slab(slabCpu, page = kmem_get_partial(cache));
            }
            return (uintptr_t) ptr;
        }
        else
        {
            /* 
            * If page pointer of cpu slab is pointing to NULL, ignore current [freelist]
            * and [page], then assign new value from the shared cache pool [node]
            */
            //kmem_fill_cpu_slab(slabCpu, page = kmem_get_partial(cache));
        }

    FAST_PATH:
        /* (2) Fast Path */
        if(!slabCpu->partial.Count()) {
            kmem_fill_cpu_slab(slabCpu, (PageFrame *) slabCpu->partial.Extract());
            if(!slabCpu->partial.Count()) {
                page = (PageFrame *) slabCpu->partial.First();

                ptr = page->freelist;
                page->freelist = (void **) *page->freelist;

                if(++page->slab_inuse == page->slab_objects) {
                    slabCpu->partial.Remove(&page->listnode);
                }
            }
            else
                goto FASTEST_PATH;
        }

    SLOW_PATH:
        /* (3) Slow Path */
        struct SlabMemoryNode *node = cache->node[0];
        if(node->nr_partial)
        {
            
        }

    SLOWEST_PATH:
        //page = SlabCache_alloc(cache);
        kmem_fill_cpu_slab(slabCpu, page = SlabCache_alloc(cache));
        ptr = page->freelist;
        page->freelist = (void **) *page->freelist;
        page->slab_inuse++;

    RETURN_POINTER:
        return (uintptr_t) ptr;
    }

    /*
    uintptr_t KernelAllocate(kstruct_index_t id)
    {
        if(id < SLAB_MAX_BLOCK_ORDER || id > 16)
            CallPanic("Kernel Struct Index out of bound.");

        return allocobj(g_cachePointers[id]);
    }
    */

    void init_kmalloc()
    {
        for(size_t idx; idx < sizeof(blockSize) / 2; idx++) {
            kmem_set_cache(
                (SlabCache *) kallocpg(4),
                idx,
                0x0
            );
        }
    }

    uintptr_t KernelAllocate(uint32_t size)
    {
        return allocobj(kmem_find_cache(size));
    }

    void KernelFree(uintptr_t addr)
    {

    }
} // namespace Memory

extern "C" void *kmalloc(size_t size)
{
    Memory::KernelAllocate(size);
}

extern "C" void kfree(void *ptr)
{
    Memory::KernelFree((uintptr_t) ptr);
}