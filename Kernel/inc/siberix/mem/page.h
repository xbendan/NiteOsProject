#pragma once

#include <stdcxx/linked-list.h>
#include <stdcxx/math.h>
#include <stdcxx/types.h>

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

namespace Kern::Mem {
    struct Page4K : Std::LinkedList<Page4K>::Entry
    {
        uint8_t _order;
        uint8_t _flags;
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
            Page4K*  _pageHead;
            void*    _kmemPool;
        };
        uint64_t _address;

        inline bool isFree()
        {
            return _flags & (PAGE_FLAG_FREE   //
                             | PAGE_FLAG_KMEM //
                             | PAGE_FLAG_SWAP);
        }

        inline uint64_t size() { return (1 << _order) * PAGE_SIZE_4K; }

        Page4K* split();
        Page4K* merge(Page4K* page);

        Page4K* merge();
    };

    struct PagePartitionModel
    {
        Page4K*  _pages;
        uint32_t _modelId;
    };

    Page4K* pageOf(uint64_t address);
}