#pragma once

#include <Memory/Page.h>
#include <Utils/List.h>
#include <Utils/Spinlock.h>
#include <Macros>
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

namespace Memory
{
    /**
     * @brief Manages the resource for specific CPU
     * 
     * This struct provides fast accessing for CPU
     * Check whether there is free objects in the cpu cache
     * before getting object from memory node.
     */
    struct KCpuCache
    {
        /* 
        * Pointer to the next object, this is a double pointer
        * because the area this pointer point to is also a pointer
        * to the next object.
        */
        void **freelist;
        pagedata_t *page;
        Utils::ListHead partial;
    };

    struct KMemoryNode
    {
        Utils::Spinlock lock;
        uint64_t nr_partial;
        Utils::ListHead partial;
    };

    struct KMemoryCache
    {
        /* The list head to connect different cache */
        Utils::ListHead listnode;
        /* Name of the object this cache managed */
        const char *name;

        /* Indicate the cache for individual CPU core */
        struct KCpuCache cpu_slab[MAX_CPU_AMOUNT];
        uint64_t flags;
        int size;
        int object_size;
        int object_align;
        int offset;
        struct KMemoryNode *node[MAX_NUMA_COUNT];
        uint64_t min_partial;
        int reserved;
    };

    void LoadKernelAllocator();
    uintptr_t KernelAllocate(uint32_t size);
    void KernelFree(uintptr_t addr);
    
    struct KCpuCache *KmGetSlabCpu(struct KMemoryCache *cache);
    struct KMemoryCache *KmFindCache(size_t size);
} // namespace Memory