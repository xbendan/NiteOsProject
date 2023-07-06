#include <Arch/x86_64/MMU.h>
#include <Mem/SlabAllocator.h>

namespace Memory
{
    uint16_t m_BlockSize[] = {8, 16, 24, 32, 48, 64, 96, 128, 192, 256, 512, 768, 1024, 1536, 2048, 4096};

    SlabAllocator::SlabAllocator() {
        auto _lambda_InitializeCache = [&](slab_cache_t* cache, uint64_t size, uint64_t flags) -> void {
            if (cache == nullptr) { return; }

            cache->size = size;
            cache->flags = flags;
            cache->reserved = ARCH_PAGE_SIZE % size;

            // for (int i = 0; i < MAX_CPU_AMOUNT; i++) {
            //     uint64_t addrVirt = 0;
            //     page_t *page = Request4KPage(cache, &addrVirt);

            //     cache->cpu_slab[i].freelist = (void **) addrVirt;
            // }

            for (int j = 0; j < MAX_NUMA_COUNT; j++)
            { cache->node[j] = (slab_node_t*)(((uint64_t)cache) + (j * sizeof(slab_node_t))); }
        };
        for (int i = 0; i < SLAB_MAX_BLOCK_ORDER; i++)
        {
            m_Caches[i] = reinterpret_cast<slab_cache_t*>(Memory::KernelAllocate4KPages(4));
            _lambda_InitializeCache(m_Caches[i], m_BlockSize[i], 0);
        }
    }

    SlabAllocator::~SlabAllocator() {}

    PageFrame* SlabAllocator::Request4KPage(slab_cache_t* cache, uint64_t* addrVirt) {
        page_t* page = nullptr;
        uint64_t addr = Memory::KernelAllocate4KPages(1);

        if (addrVirt != nullptr) { *addrVirt = addr; }

        page->flags |= PFLAGS_KMEM;
        page->slab_cache = cache;
        page->freelist = (void**)addr;

        page->slab_objects = ((ARCH_PAGE_SIZE - cache->reserved) / cache->size);
        page->slab_inuse = 0;

        for (int i = 0; i < page->slab_objects - 1; i++)
        {
            // *((uint64_t *)(addr + i * cache->size)) = (uint64_t)(addr + (i + 1) * cache->size);
            uint64_t next = addr + cache->size;
            uint64_t* p = reinterpret_cast<uint64_t*>(addr);
            *p = next;
            addr += cache->size;
        }

        return page;
    }

    uintptr_t SlabAllocator::Allocate(size_t size) {
        if (size > ARCH_PAGE_SIZE) { return KernelAllocate4KPages(ALIGN_UP(size, ARCH_PAGE_SIZE) / ARCH_PAGE_SIZE); }

        /*
         * Find the cache with suitable size
         * request size >= cache size
         */
        slab_cache_t* cache = nullptr;
        for (int i = 0; i < 16; i++)
            if (m_BlockSize[i] >= size)
            {
                cache = m_Caches[i];
                break;
            }
        Objects::Validate(cache != nullptr, "[Slab] Not suitable cache found.");

        slab_cpu_t* cpu = &cache->cpu_slab[CurrentCPU()];
        page_t* page = cpu->page;
        uint64_t address;

        if (!page || page->slab_inuse >= page->slab_objects) page = cpu->page = this->Request4KPage(cache);

        /*
         * The value of 'page->freelist' indicates where the next object is,
         * Move it to variable 'address' as the return value.
         * Assign the new pointer point to the next object.
         *
         * Finally, check whether the page has ran out of space ('inuse' equals to 'objects')
         * If yes, request new physical page from page frame allocator.
         */
        address = (uint64_t)page->freelist;

        page->freelist = (void**)*(page->freelist);
        /* Check whether the objects in this page is running out */
        if (++page->slab_inuse == page->slab_objects) { cpu->page = Request4KPage(cache); }

        memset((void*)address, 0, cache->size);
        return address;
    }

    void SlabAllocator::Free(uintptr_t address) {}
} // namespace Memory