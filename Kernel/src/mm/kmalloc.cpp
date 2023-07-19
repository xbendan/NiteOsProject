#include <siberix/core/runtimes.hpp>
#include <siberix/mm/slab.hpp>
#include <utils/alignment.h>

namespace Memory
{
    u16 blockSize[] = {8, 16, 24, 32, 48, 64, 96, 128, 192, 256, 512, 768, 1024, 1536, 2048, 4096};

    SlabAlloc::SlabAlloc() {
        for (int i = 0; i < SLAB_MAX_BLOCK_ORDER; i++)
            caches[i] = new SlabCache(blockSize[i], 0);
    }

    SlabAlloc::~SlabAlloc() {}

    Pageframe* SlabAlloc::cache(u32 size) {
        SlabCache* cache;
        for (int i = 0; i < SLAB_MAX_BLOCK_ORDER; i++)
        {
            if (blockSize[i] >= size)
            {
                cache = caches[i];
                break;
            }
        }
        return cache;
    }

    u64 SlabAlloc::alloc(u64 size) {
        if (size > PAGE_SIZE_4K)
            return exec()->getMemory().alloc4KPages(alignUp(size, PAGE_SIZE_4K));
        /*
         * Find the cache with suitable size
         * request size >= cache size
         */
        SlabCache* cache = getCache(size);
        SlabCpuCache* cpu = &cache->cpus[0];
        Pageframe* page = cpu->page;
        u64 address = 0;

        if (!page || page->slabInuse >= page->slabObjects)
            page = cpu->page = cache->request4KPage(cache);

        /*
         * The value of 'page->freelist' indicates where the next object is,
         * Move it to variable 'address' as the return value.
         * Assign the new pointer point to the next object.
         *
         * Finally, check whether the page has ran out of space ('inuse' equals to 'objects')
         * If yes, request new physical page from page frame allocator.
         */
        address = (u64)page->freelist;
        page->freelist = (void**)*(page->freelist);

        /* Check whether the objects in this page is running out */
        if (++page->slabInuse == page->slabObjects)
            cpu->page = cache->request4KPage(cache);

        memset((void*)address, 0, cache->size);
        return address;
    }

    void SlabAlloc::free(u64 address) {}

    SlabCache::SlabCache(u64 size, u64 flags) {
        this->size = size;
        this->flags = flags;
        this->reserved = (PAGE_SIZE_4K % size);
        for (int i = 0; i < 256; i++)
        {
            u64 v = 0;
            Pageframe* page = request4KPage(&v);
            cpus[i].freelist = (void**)v;
        }
    }

    SlabCache::~SlabCache() {}

    Pageframe* SlabCache::request4KPage(u64* addrVirt) {
        Pageframe* page;
        u64 addr = exec()->getMemory().alloc4KPages(1, &page);

        if (addrVirt != nullptr)
        { *addrVirt = addr; }
        page->flags |= PFLAGS_KMEM;
        page->slabCache = this;
        page->freelist = (void**)addr;
        page->slabObjects = ((PAGE_SIZE_4K - reserved) / size);
        page->slabInuse = 0;

        for (int i = 0; i < page->slabObjects - 1; i++)
        {
            u64 next = addr + size;
            u64* p = reinterpret_cast<u64*>(addr);
            *p = next;
            addr += size;
        }

        return page;
    }
} // namespace Memory