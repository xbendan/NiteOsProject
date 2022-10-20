#include <Memory/Slub.h>
#include <Process/Scheduler.h>
#include <Kernel>

#ifdef ARCH_X86_64
#include <Arch/x86_64/Paging.h>
#include <Arch/x86_64/CPU.h>
#endif

namespace Memory
{
    const uint16_t blockSize[] = {
        8,      16,     32,     48,     64,     96,     128,    192, 
        256,    512,    768,    1024,   1536,   2048,   4096,   8192,
        sizeof(Thread)
    };
    static KMemoryCache *g_cachePointers[SLAB_MAX_BLOCK_ORDER];
    static Utils::LinkedList<KMemoryCache> g_cacheList;

    void KmSetCacheData(struct KMemoryCache *cache, int order, uint64_t flags)
    {
        if(cache == NULL)
        {
            CallPanic("INVALID ADDRESS");
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
            cache->node[idx] = (struct KMemoryNode *)(((uint64_t) cache) + (idx * sizeof(struct KMemoryNode)));

        LinkedListAppend(&g_cacheList, &cache->listnode);
        g_cachePointers[order] = cache;
    }

    void KmSetPageData(struct KMemoryCache *cache, pagedata_t *page, uintptr_t virt)
    {
        page->kmem = true;
        page->slabCache = cache;
        page->freelist = (void **) virt;

        page->slab_objects = ((ARCH_PAGE_SIZE - cache->reserved) / cache->size);
        page->slab_inuse = 0;

        for(size_t idx = 0; idx < page->slab_objects - 1; idx++)
            *((uint64_t *)(virt + (idx * cache->size))) = (uint64_t)(virt + ((idx + 1) * cache->size));
    }

    pagedata_t *AttemptRetrieveNewPage(struct KMemoryCache *cache)
    {
        uintptr_t virt = AllocatePages(Proc::GetKernelProcess(), 1);
        if(virt == 0x0)
            CallPanic("");
        else
        {
            pagedata_t *page = GetPage(ConvertVirtToPhys(Memory::GetKernelPages(), virt));
            KmSetPageData(cache, page, virt);
            return page;
        }
    }

    struct KCpuCache *KmGetSlabCpu(struct KMemoryCache *cache)
    {
        return &cache->cpu_slab[CPU_CORE_ID];
    }

    struct KMemoryCache *KmFindCache(size_t size)
    {
        struct KMemoryCache *cache;

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
     * @return pagedata_t* 
     */
    pagedata_t *RetrievePartialPage(struct KMemoryCache *cache)
    {
        struct KMemoryNode *node = cache->node[0];
        struct PageData *page;

        node->lock.Acquire();
        
        if(node->nr_partial == 0)
            page = AttemptRetrieveNewPage(cache);
        else
        {
            pagedata_t *page = (pagedata_t *) node->partial.next;
            node->nr_partial--;
            LinkedListRemove(&page->listnode);
        }

        node->lock.Release();
        return page;
    }

    /**
     * @brief 
     * 
     * @param slabCpu 
     * @param page 
     */
    void KmAssignCpuSlab(struct KCpuCache *slabCpu, pagedata_t *page)
    {
        LinkedListRemove(&page->listnode);

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
    uintptr_t AllocateObject(struct KMemoryCache *cache)
    {
        if (cache == NULL)
            CallPanic("Null Pointer for cache while allocating kernel memory.");
        
        KCpuCache *slabCpu = KmGetSlabCpu(cache);
        Memory::PageData *page = slabCpu->page;
        void **ptr;

    FASTEST_PATH:
        /*
        * Try to use the object in cpu slab first
        */
        ptr = slabCpu->freelist;

        /* (1) Fastest Path */
        if(ptr != NULL)
        {
            /* Assign freelist point to the next object (might be NULL) */
            page->freelist = slabCpu->freelist = *ptr;

            /* Check whether the objects in this page is running out */
            if(++page->slab_inuse == page->slab_objects)
            {
                KmAssignCpuSlab(slabCpu, page = RetrievePartialPage(cache));
                //page = RetrievePartialPage(cache);
            }
            goto RETURN_POINTER;
        }
        else
        {
            /* 
            * If page pointer of cpu slab is pointing to NULL, ignore current [freelist]
            * and [page], then assign new value from the shared cache pool [node]
            */
            //KmAssignCpuSlab(slabCpu, page = RetrievePartialPage(cache));
        }

    FAST_PATH:
        /* (2) Fast Path */
        if(slabCpu->partial.next != NULL)
        {
            KmAssignCpuSlab(slabCpu, (pagedata_t *) slabCpu->partial.next);
            if(slabCpu->partial.next != NULL)
            {
                page = (pagedata_t *) slabCpu->partial.next;

                ptr = page->freelist;
                page->freelist = *page->freelist;

                if(++page->slab_inuse == page->slab_objects)
                {
                    Utils::LinkedList::Remove(&page->listnode);
                }
            }
            else
                goto FASTEST_PATH;
        }

    SLOW_PATH:
        /* (3) Slow Path */
        struct KMemoryNode *node = cache->node[0];
        if(node->nr_partial)
        {
            
        }

    SLOWEST_PATH:
        //page = AttemptRetrieveNewPage(cache);
        KmAssignCpuSlab(slabCpu, page = AttemptRetrieveNewPage(cache));
        ptr = page->freelist;
        page->freelist = *page->freelist;
        page->slab_inuse++;

    RETURN_POINTER:
        return (uintptr_t) ptr;

    }

    /*
    uintptr_t KernelAllocate(kstruct_index_t id)
    {
        if(id < SLAB_MAX_BLOCK_ORDER || id > 16)
            CallPanic("Kernel Struct Index out of bound.");

        return AllocateObject(g_cachePointers[id]);
    }
    */

    void LoadKernelAllocator()
    {
        for(size_t idx; idx < sizeof(blockSize) / 2; idx++)
        {
            KmSetCacheData(
                (KMemoryCache *) AllocatePages(Proc::GetKernelProcess(), 4),
                idx,
                0x0
            );
        }
    }

    uintptr_t KernelAllocate(uint32_t size)
    {
        return AllocateObject(KmFindCache(size));
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
    Memory::KernelFree(ptr);
}