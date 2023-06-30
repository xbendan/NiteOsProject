#include <Init/BootInfo.h>
#include <Mem/Memory.h>
#include <Mem/Page.h>
#include <Mem/SlabAllocator.h>
#include <Mem/MemZone.h>
#include <Proc/Scheduler.h>
#include <Utils/Range.h>
#include <Mem/AddressSpace.h>

#ifdef ARCH_X86_64
#include <Arch/x86_64/MMU.h>
#endif

namespace Memory
{
    /// @brief memory usage statistics
    /// total, available, present, swapped, 
    uint64_t g_Statistics[8];
    SizedArrayList<ValueRange, MEMORY_MAP_LIMIT> g_Blocks;
    SizedArrayList<Section, SPARSEMEM_LIMIT> g_Sections;
    SlabAllocator *g_KernelAllocator;

    void Initialize() {
        BootInfoMemory *mem = &bootInfo.m_Memory;

        g_Statistics[MemValTotal] = mem->m_TotalSize;
        g_Statistics[MemValAvailable] = mem->m_Usable;

        System::Out("111");

        Paging::InitializeVirtualMemory();

        for (int i = 0; i < mem->m_MemoryMapSize; i++)
        {
            MemoryMapEntry *mapEntry = &mem->m_MemoryMapEntries[i];
            if (mapEntry->m_AddrStart < 0x100000)
                mapEntry->m_AddrStart = 0x100000;

            if ((mapEntry->m_Type != MemoryMapEntryTypeAvailable) ||
                (mapEntry->m_AddrStart > mapEntry->m_AddrEnd - ARCH_PAGE_SIZE))
            {
                continue;
            }

            g_Blocks.Add({ mapEntry->m_AddrStart, mapEntry->m_AddrEnd });
        }

        auto _lambda_SectionPresent = [](int nid, ValueRange &vals) -> void 
        {
            if (vals.End - vals.Start < ARCH_PAGE_SIZE)
                return;

            size_t pageAmount = PAGES_PER_SECTION * 64;
            uint64_t current = vals.Start;
            while (current < vals.End)
            {
                Section *section = &(g_Sections[current >> SECTION_SHIFT]);
                if (!section->pages)
                {
                    uint64_t virt = Paging::KernelAllocate4KPages(pageAmount / ARCH_PAGE_SIZE);
                    uint64_t phys;
                    g_Blocks.ForEach([&](ValueRange vals0, int index) -> void
                    {
                        if ((vals0.End - vals0.Start) >= pageAmount)
                        {
                            phys = vals0.Start;
                            vals0.Start += pageAmount;
                        }
                    });
                    Objects::Validate(phys && virt, "Failed to allocate memory in early env.");

                    section->pages = reinterpret_cast<page_t *>(virt);
                    Paging::KernelMapVirtualMemory4K(phys, virt, pageAmount / ARCH_PAGE_SIZE);
                }

                current += 0x40000000;
            }
        };
        g_Blocks.ForEach([&](ValueRange &vals, int index) -> void { _lambda_SectionPresent(index, vals); });

        BuddyInit();
        uint64_t slabPhys = KernelAllocate4KPages(1);
        System::Out("%x", slabPhys);
        *(g_KernelAllocator = reinterpret_cast<SlabAllocator *>(slabPhys)) = SlabAllocator();
    }
} // namespace Memory