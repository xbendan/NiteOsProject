#include <Init/BootInfo.h>
#include <Memory/Page.h>
#include <Memory/Memory.h>

namespace Memory
{
    List<PageData *> pageList[PAGE_MAX_ORDER + 1];
    //PageList pageList[PAGE_MAX_ORDER + 1];

    static inline void WRITE_PAGE_DATA(PageData *pages, uint64_t addr)
    {
        for (size_t pageIndex = 0; 
                pageIndex < 1024; 
                pageIndex++)
        {
            pages[pageIndex].free = 1;
            pages[pageIndex].addr = addr + (pageIndex * ARCH_PAGE_SIZE);
            //pages[idx].first_page = pages;
        }
        
        pages->order = PAGE_MAX_ORDER;
        pageList[PAGE_MAX_ORDER - 1].dataList
        /*
        LinkedListAppend(
            &pf_freelist[PAGE_MAX_ORDER - 1].handle,
            &pages->listnode
        );
        */
        pageList[PAGE_MAX_ORDER - 1].count++;
    }

    void LoadPageAllocator()
    {
        int mapSize = &GetBootInfo()->m_Memory.m_MemoryMapSize;
        MemoryMapEntry *mapEntries = &GetBootInfo()->m_Memory.m_MemoryMapEntries;
        uint64_t blockStartAddr;
        uint64_t blockEndAddr;

        for (int mapIndex = 0;
                mapIndex < mapSize;
                mapIndex++)
        {
            MemoryMapEntry *mapEntry = &mapEntries[mapIndex];
            size_t entrySize = (mapEntry->m_AddrEnd - mapEntry->m_AddrStart);
            switch (mapEntry->m_Type)
            {
            case MemoryMapEntryTypeAvailable:
                memStats.available += size;
                break;
            case MemoryMapEntryTypeKernel:
                memStats.allocated += size;
                memStats.inuse += size;
                MarkPagesUsed((range_t){
                    .start = ALIGN_DOWN(mapEntry->m_AddrStart, ARCH_PAGE_SIZE),
                    .end = ALIGN_UP(mapEntry->m_AddrEnd, ARCH_PAGE_SIZE)
                });
                continue;
            default:
                continue;
            }

            blockStartAddr = ALIGN_UP(mapEntry.m_AddrStart, PAGE_MAX_SIZE);
            blockEndAddr = ALIGN_DOWN(mapEntry.m_AddrEnd, PAGE_MAX_SIZE);

            if(blockStartAddr >= blockEndAddr)
            {
                return;
            }
            uint64_t addr = blockStartAddr;

            while (addr < blockEndAddr - PAGE_MAX_SIZE)
            {
                if(addr < 0x100000)
                    continue;

                PageFrame *allocSpace = AllocatePages(4);
                if(allocSpace != NULL)
                {
                    uint64_t pages = (addr / ARCH_PAGE_SIZE * sizeof(PageFrame)) + KERNEL_PHYSICAL_PAGES;
                    MapVirtualAddress(
                        GetKernelPages(),
                        AllocatePages(4)->addr,
                        pages,
                        16,
                        PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE
                    )
                }
            }
        }
    }

    pageframe_t* Allocate(size_t size)
    {

    }

    pageframe_t* AllocatePages(uint8_t order)
    {

    }

    void Free(uintptr_t addr);
    void FreePages(pageframe_t* pf);
    void MarkPagesUsed(range_t range);
    pageframe_t* GetPage(uintptr_t addr);
    pageframe_t* ExpandPage(pageframe_t* pf);
    pageframe_t* CombinePage(pageframe_t *pf);
    void CombinePages(pageframe_t *lpage, pageframe_t *rpage);
} // namespace Memory
