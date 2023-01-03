#include <mm/page.h>
#include <mm/mmzone.h>

#ifdef ARCH_X86_64
#include <Arch/x86_64/MMU.h>
#endif

namespace Memory {
    buddyzone_t *zones;

    void BuddyInit() {
        bootmem_t *bootMemo = &bootInfo->m_Memory;
        const size_t amount = sizeof(page_t) * PAGES_PER_SECTION / ARCH_PAGE_SIZE;

        for (int i = 0; i < bootMemo->m_MemoryMapSize; i++) {
            memmap_entry_t *mapEntry = &(bootMemo->m_MemoryMapEntries[i]);

            for (addr = ALIGN_UP(mapEntry->m_AddrStart, ARCH_PAGE_SIZE);
                    addr < ALIGN_DOWN(mapEntry->m_AddrEnd, ARCH_PAGE_SIZE); 
                    addr += SIZE_1G) {
                uint64_t virt = Memory::Mapping::KernelAllocate4KPages(amount);
                uint64_t phys = MemblockAllocate(amount);
                
                sections[addr >> SECTION_SHIFT].pages = (page_t *) virt;
                Memory::Mapping::KernelMapVirtualAddress(phys, virt, amount);
            }
        }
    }

    page_t *Allocate4KPages(size_t amount);
    void Free4KPages(uintptr_t address);
} // namespace Memory
