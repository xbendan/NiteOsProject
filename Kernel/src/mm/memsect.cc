#include <mm/mmzone.h>
#include <kern.h>

using namespace Memory;

namespace Memory::Model
{
    mem_section_t sections[SPARSEMEM_LIMIT];

    void MemsectPresent(int nid, uint64_t pfnStart, uint64_t pfnEnd) {
        if(pfnEnd - pfnStart < 1) {
            return;
        }

        for (; pfnStart < pfnEnd; pfnStart += PAGES_PER_SECTION) {
            uint64_t virt = ManagementUnit::KernelAllocate4KPages(amount), phys = MemblockAllocate(amount);
            if (phys && virt) {
                sections[m_PageNum >> PAGE_SECTION_SHIFT].pages = virt;
                ManagementUnit::KernelMapVirtualAddress(phys, virt, amount);
            } else CallPanic("");
        }
    }

    void MemsectsInit() {
        /* Initialize mem section and allocate pages */
        const size_t amount = sizeof(page_t) * PAGES_PER_SECTION / ARCH_PAGE_SIZE;
        int index = 0;
        while (memblocks[index].m_PageNum) {
            mem_block_t *block = &(memblocks[index]);
            uint64_t limit = block->m_PageNum + block->m_Amount;
            MemsectPresent(0,
                block->m_PageNum,
                block->m_PageNum + block->m_Amount);
            index++;
        }
    }
} // namespace Memory::Model
