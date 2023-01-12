#include <mm/mmzone.h>
#include <kern.h>
#include <address.h>

using namespace Memory;

namespace Memory::Model
{
    mem_section_t sections[SPARSEMEM_LIMIT];

    void MemsectPresent(int nid, uint64_t addrStart, uint64_t addrEnd) {
        if(addrEnd - addrStart < ARCH_PAGE_SIZE) {
            return;
        }

        const size_t amount = 64 * PAGES_PER_SECTION / ARCH_PAGE_SIZE;
        for (; addrStart < addrStart; addrStart += 0x40000000) {
            mem_section_t *section = &(sections[addrStart >> SECTION_SHIFT]);
            if (section->pages) {
                continue;
            }

            uint64_t virt = ManagementUnit::KernelAllocate4KPages(amount);
            uint64_t phys = (uint64_t) MemblockAllocate(amount);
            if (phys && virt) {
                section->pages = reinterpret_cast<page_t *>(virt);
                ManagementUnit::KernelMapVirtualMemory4K(phys, virt, amount);
            } else {
                CallPanic("");
            }
        }
    }

    void MemsectsInit() {
        /* Initialize mem section and allocate pages */
        int index = 0;
        memset(&sections, 0, sizeof(mem_section_t) * SPARSEMEM_LIMIT);
        while (memblocks[index].m_AddrStart) {
            mem_block_t *block = &(memblocks[index++]);
            MemsectPresent(0, block->m_AddrStart, block->m_AddrEnd);
        }
    }
} // namespace Memory::Model
