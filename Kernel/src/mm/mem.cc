#include <init/bootinfo.h>
#include <mm/mem.h>
#include <mm/page.h>
#include <mm/slab.h>
#include <mm/mmzone.h>
#include <proc/sched.h>
#include <kern.h>
#include <address.h>

#ifdef ARCH_X86_64
#include <Arch/x86_64/MMU.h>
#endif

namespace Memory
{
    /// @brief memory usage statistics
    /// total, available, present, swapped, 
    uint64_t stat[8];

    void Initialize() {
        bootmem_t *mem = &bootInfo->m_Memory;

        stat[MEMVAL_TOTAL] = mem->m_TotalSize;
        stat[MEMVAL_AVAILABLE] = bootInfo.m_Memory.m_Usable;

        ManagementUnit::InitializeVirtualMemory();

        Model::MemblocksInit();
        Model::MemsectsInit();

        BuddyInit();

        /* 
         * Setup memory block from memory map entries as 
         * early page frame allocating.
         */
        int blocks = 0;
        for (int i = 0; i < bootMemo->m_MemoryMapSize; i++) {
            memmap_entry_t *mapEntry = &(bootMemo->m_MemoryMapEntries[i]);
            if (mapEntry->m_Type != MemoryMapEntryAvailable)
                continue;
            
            uint64_t addrStart = ALIGN_UP(mapEntry->m_AddrStart, ARCH_PAGE_SIZE);
            uint64_t addrEnd = ALIGN_DOWN(mapEntry->m_AddrEnd, ARCH_PAGE_SIZE);

            if(addrEnd - addrStart < ARCH_PAGE_SIZE)
                continue;

            Model::memblocks[blockIndex] = (memblock_t) {
                .m_PageNum = addrStart >> PAGE_SHIFT;
                .amount = (addrEnd - addrStart) / ARCH_PAGE_SIZE;
            };
            blocks++;
        }
        
        /* Initialize mem section and allocate pages */
        const size_t amount = sizeof(page_t) * PAGES_PER_SECTION / ARCH_PAGE_SIZE;
        int index = 0;
        while (memblocks[index].m_PageNum && index < blocks) {
            memblock_t *block = &(memblocks[index]);
            uint64_t limit = block->m_PageNum + block->amount;
            for (uint64_t m_PageNum = block->m_PageNum; m_PageNum < limit; m_PageNum += PAGES_PER_SECTION) {
                uint64_t virt = ManagementUnit::KernelAllocate4KPages(amount), phys = Model::MemblockAllocate(amount);
                if(phys && virt) {
                    sections[m_PageNum >> PAGE_SECTION_SHIFT].pages = virt;
                    ManagementUnit::KernelMapVirtualAddress(phys, virt, amount);
                } else {
                    // panic here.
                    return;
                }
            }
            index++;
        }

        BuddyInit();

        SlabInit();
    }
} // namespace Memory