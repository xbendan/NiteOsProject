#include <Mem/SlabAllocator.h>
#include <Mem/KMemAlloc.h>
#include <Proc/Process.h>
#include <Proc/Scheduler.h>
#include <libkern/objects.h>
#include <Utils/LinkedList.h>
#include <kern.h>
#include <System.h>

#ifdef ARCH_X86_64
#include <Arch/x86_64/mmu.h>
#include <Arch/x86_64/cpu.h>
#endif

namespace Memory
{
    const uint16_t blockSize[] = {
        8,      16,     24,     32,     48,     64,     96,     128, 
        192,    256,    512,    768,    1024,   1536,   2048,   4096,
        sizeof(Task::Thread)
    };
    const uint16_t blockArrayLength = sizeof(blockSize) / sizeof(uint16_t);
    slab_cache_t *caches[SLAB_MAX_BLOCK_ORDER];

    void SetCache(slab_cache_t *cache, int order, uint64_t flags) {
        if (cache == nullptr) {
            return;
        }

        int size = blockSize[size];
        cache->size = size;
        cache->flags = flags;
        cache->reserved = ARCH_PAGE_SIZE / size;

        for (int i = 0; i < MAX_CPU_AMOUNT; i++) {
            uint64_t addrVirt = 0;
            page_t *page = Request4KPageMapped(cache, &addrVirt);
            
            cache->cpu_slab[i].freelist = (void **) addrVirt;
        }

        for (int j = 0; j < MAX_NUMA_COUNT; j++) {
            cache->node[j] = (slab_node_t *)(((uint64_t) cache) + (j * sizeof(slab_node_t)));
        }

        caches[order] = cache;
    }

    void SetPageSlub(slab_cache_t *cache, page_t *page, uintptr_t virt) {
        page->flags |= PFLAGS_KMEM;
        page->slab_cache = cache;
        page->freelist = (void **) virt;

        page->slab_objects = ((ARCH_PAGE_SIZE - cache->reserved) / cache->size);
        page->slab_inuse = 0;

        for (int i = 0; i < page->slab_objects - 1; i++) {
            *((uint64_t *)(virt + (i * cache->size))) = (uint64_t)(virt + ((i + 1) * cache->size));
        }
    }

    page_t *Request4KPageMapped(slab_cache_t *cache, uint64_t *addrVirt)
    {
        page_t *page = Request4KPage(cache);
        uintptr_t _addrVirt = ManagementUnit::KernelAllocate4KPages(1);
        ManagementUnit::KernelMapVirtualMemory4K(page->addr, _addrVirt, 1);

        if (!Objects::IsNull(addrVirt)) {
            *addrVirt = _addrVirt;
        }

        SetPageSlub(cache, page, _addrVirt);

        return page;
    }

    page_t *Request4KPage(slab_cache_t *cache) {
        page_t *page = AllocatePhysMemory4K(1);

        if (Objects::IsNull(page)) {
            System::Panic("Slub allocator failed to get page.");
        }
        
        return page;
    }

    slab_cpu_t *SlubGetCPU(slab_cache_t *cache) {
        return &cache->cpu_slab[CPU_CORE_ID];
    }

    slab_cache_t *SlubGetCache(size_t size) {
        slab_cache_t *cache;
        if (size > SLAB_MAX_SIZE) {
            System::Out("Size %u is greater than max size.", size);
            return nullptr;
        }

        for (int order = 0; order < 16; order++) {
            if (blockSize[order] > size) {
                cache = caches[order];
                break;
            }
        }
        return cache;
    }

    page_t *SlubGetPartial(slab_cache_t *cache) {
        slab_node_t *node = cache->node[0];
        page_t *page;

        node->lock.Acquire();

        page = node->partial.Count() ?
            ((page_t *) node->partial.Extract()) :
            Request4KPageMapped(cache, nullptr);

        node->lock.Release();
        return page;
    }

    page_t SlabFillCpuSlot(slab_cpu_t *slab_cpu, page_t *page) {
        slab_cpu->freelist = page->freelist;
        slab_cpu->page = page;

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
    uintptr_t AllocateKernelObject(slab_cache_t *cache) {
        if (!cache) {
            System::Panic("Null Pointer for cache while allocating kernel memory.");
        }

        slab_cpu_t *slab_cpu = SlubGetCPU(cache);
        page_t *page = slab_cpu->page;
        void **pointer;

FastestPath:
        /* Try to use the object in cpu slab first */
        pointer = slab_cpu->freelist;
        /* (1) Fastest Path */
        if (pointer) {
            /* Assign freelist point to the next object (might be NULL) */
            page->freelist = slab_cpu->freelist = (void **) *pointer;
            memset((void *) pointer, 0, cache->size);

            /* Check whether the objects in this page is running out */
            if (++page->slab_inuse == page->slab_objects) {
                SlabFillCpuSlot(slab_cpu, page = SlubGetPartial(cache));
            }
            return (uintptr_t) pointer;
        } else {
            /* 
             * If page pointer of cpu slab is pointing to NULL, ignore current [freelist]
             * and [page], then assign new value from the shared cache pool [node]
             */
            // SlabFillCpuSlot(slab_cpu, page = SlubGetPartial(cache));
        }

        /* (2) Fast Path */
        if(!slab_cpu->partial.Count()) {
            SlabFillCpuSlot(slab_cpu, (page_t *) slab_cpu->partial.Extract());
            if (!slab_cpu->partial.Count()) {
                page = (page_t *) slab_cpu->partial.First();

                pointer = page->freelist;
                page->freelist = (void **) *page->freelist;

                if (++page->slab_inuse == page->slab_objects) {
                    slab_cpu->partial.Remove(&page->lru);
                }
                return (uintptr_t) pointer;
            } else goto FastestPath;
        }

SlowPath:
        /* (3) Slow Path */
        slab_node_t *node = cache->node[0];
        if (node->partial.Count()) {

        }

SlowestPath:
        //page = SlabCache_alloc(cache);
        SlabFillCpuSlot(slab_cpu, page = Request4KPageMapped(cache, nullptr));
        pointer = page->freelist;
        page->freelist = (void **) *page->freelist;
        page->slab_inuse++;

        System::Out("KObjAlloc");
        return (uintptr_t) pointer;
    }

    void SlubInit() {
        for (int i = 0; i < blockArrayLength; i++) {
            uint64_t address = Memory::KernelAllocate4KPages(1);
            SetCache(reinterpret_cast<slab_cache_t *>(address), i, 0);
        }
    }

    uintptr_t AllocateKernelObject(uint32_t size) {
        return (size > ARCH_PAGE_SIZE)
            ? Allocate4KPages(ALIGN_UP(size, ARCH_PAGE_SIZE) / ARCH_PAGE_SIZE)
            : AllocateKernelObject(SlubGetCache(size));
    }

    void FreeKernelObject(uintptr_t addr) {

    }
} // namespace Memory