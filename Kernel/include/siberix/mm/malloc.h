#pragma once

#include <siberix/mm/types.h>

namespace Memory {
    class MemoryAlloc {
    public:
        virtual u64  alloc(u64 size)   = 0;
        virtual void free(u64 address) = 0;
    };

    class PageAlloc {
    public:
        virtual Pageframe* allocatePhysMemory4K(u64 amount)  = 0;
        virtual void       freePhysMemory4K(u64 address)     = 0;
        virtual void       freePhysMemory4K(Pageframe* page) = 0;
    };
}  // namespace Memory