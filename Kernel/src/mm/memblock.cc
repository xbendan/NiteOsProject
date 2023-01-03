#include <mm/mem.h>

namespace Memory::Model
{
    memblock_t memblocks[MEMORY_MAP_LIMIT];

    void *MemblockAllocate(size_t amount) {
        int blockIndex = 0;
        void *ptr = nullptr;
        while (blockIndex < MEMORY_MAP_LIMIT && memblocks[blockIndex].pfn) {
            memblock_t *memblock = &(memblocks[blockIndex]);

            if(memblock->amount < amount)
                continue;

            ptr = (void *)(memblock->pfn * ARCH_PAGE_SIZE);

            memblock->amount -= amount;
            memblock->pfn += amount;
            break;
        }
        return ptr;
    }
} // namespace Memory
