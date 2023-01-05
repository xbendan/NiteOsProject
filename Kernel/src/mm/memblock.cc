#include <mm/mem.h>

namespace Memory::Model
{
    mem_block_t memblocks[MEMORY_MAP_LIMIT];

    void MemblocksInit(bootmem_t *memdat) {
        /* 
         * Setup memory block from memory map entries as 
         * early page frame allocating.
         */
        int blocks = 0;
        for (int i = 0; i < memdat->m_MemoryMapSize; i++) {
            memmap_entry_t *mapEntry = &(memdat->m_MemoryMapEntries[i]);
            if (mapEntry->m_Type != MemoryMapEntryAvailable)
                continue;
            
            uint64_t addrStart = ALIGN_UP(mapEntry->m_AddrStart, ARCH_PAGE_SIZE);
            uint64_t addrEnd = ALIGN_DOWN(mapEntry->m_AddrEnd, ARCH_PAGE_SIZE);

            if(addrEnd - addrStart < ARCH_PAGE_SIZE)
                continue;

            memblocks[blocks] = (mem_block_t) {
                .m_PageNum = addrStart >> PAGE_SHIFT;
                .m_Amount = (addrEnd - addrStart) / ARCH_PAGE_SIZE;
            };
            blocks++;
        }
    }

    void *MemblockAllocate(size_t amount) {
        int blockIndex = 0;
        void *ptr = nullptr;
        while (blockIndex < MEMORY_MAP_LIMIT && memblocks[blockIndex].pfn) {
            mem_block_t *memblock = &(memblocks[blockIndex]);

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
