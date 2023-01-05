#pragma once

#include <Memory/Page.h>
#include <utils/list.h>
#include <Utils/Spinlock.h>
#include <macros>
#ifdef ARCH_X86_64
    #include <Arch/x86_64/CPU.h>
#elif ARCH_AARCH64
    
#elif ARCH_RISCV

#endif

#define SLAB_MAX_BLOCK_ORDER        16
#define SLAB_MAX_STRUCT_ORDER       1
#define SLAB_MAX_SIZE               8192
#define MAX_NUMA_COUNT              4

uint8_t BLOCK_ORDER(size_t size);

using namespace Utils;

namespace Memory
{
    /**
     * @brief Manages the resource for specific CPU
     * 
     * This struct provides fast accessing for CPU
     * Check whether there is free objects in the cpu cache
     * before getting object from memory node.
     */
    typedef struct SlabCpuCache
    {
        /* 
        * Pointer to the next object, this is a double pointer
        * because the area this pointer point to is also a pointer
        * to the next object.
        */
        void **freelist;
        page_t *page;
        Utils::LinkedList<page_t> partial;
    } slab_cpu_t;

    typedef struct SlabMemoryNode
    {
        Utils::Spinlock lock;
        uint64_t nr_partial;
        Utils::LinkedList<page_t> partial;
    } slab_node_t;

    typedef struct SlabCache
    {
        /* The list head to connect different cache */
        listhead_t lru;
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

    void SlabInit();
    uintptr_t kmalloc(uint32_t size);
    void kfree(uintptr_t addr);
    
    slab_cpu_t *FindCpuCache(struct SlabCpuCache *cache);
    slab_cache_t *FindMemCache(size_t size);
} // namespace Memory