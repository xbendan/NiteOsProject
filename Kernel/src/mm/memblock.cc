#include <mm/mem.h>
#include <mm/mmzone.h>

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
            if (mapEntry->m_Type != MemoryMapEntryTypeAvailable)
                continue;
            
            uint64_t addrStart = ALIGN_UP(mapEntry->m_AddrStart, ARCH_PAGE_SIZE);
            uint64_t addrEnd = ALIGN_DOWN(mapEntry->m_AddrEnd, ARCH_PAGE_SIZE);
            
            if (addrStart < LOWMEM_RESERVED) 
                addrStart = LOWMEM_RESERVED;

            if(addrStart > addrEnd - ARCH_PAGE_SIZE)
                continue;

            memblocks[blocks] = { .m_AddrStart = addrStart, .m_AddrEnd = addrEnd };
            blocks++;
        }
        memblocks[blocks].m_AddrStart = 0; 
    }

    void *MemblockAllocate(size_t amount) {
        int blockIndex = 0;
        void *ptr = nullptr;
        while (blockIndex < MEMORY_MAP_LIMIT && memblocks[blockIndex].m_AddrStart) {
            mem_block_t *memblock = &(memblocks[blockIndex++]);

            if((memblock->m_AddrEnd - memblock->m_AddrStart) / ARCH_PAGE_SIZE < amount)
                continue;

            ptr = (void *)(memblock->m_AddrStart);

            memblock->m_AddrStart += amount * ARCH_PAGE_SIZE;
            break;
        }
        return ptr;
    }
} // namespace Memory
