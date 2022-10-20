#include <Init/BootInfo.h>
#include <Process/Scheduler.h>
#include <Memory/Page.h>
#include <Memory/Memory.h>

namespace Memory
{
    Utils::LinkedList<PageData *> pageList[PAGE_MAX_ORDER + 1];
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
            /*
             * 
             *
             */
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
                MarkPagesUsed((Range){
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

    PageData* AllocatePages(uint8_t order)
    {
        if(order > PAGE_MAX_ORDER || order < PAGE_MIN_ORDER)
        {
            return NULL;
        }

        PageData *page;
        uint8_t m_order = order;
        
        PageList *selectedPageList = NULL;
        while(m_order <= PAGE_MAX_ORDER)
        {
            if(pageList[m_order].m_Count)
            {
                selectedPageList = &pf_freelist[m_order];
                break;
            }
            m_order++;
        }

        if(selectedPageList == NULL)
        {
            // panic! (no enough memory space.)
            // or try to swap pages, kill processes
            return NULL;
        }

    SELECT_PAGE:
        page = (PageData*)(selectedPageList->handle.next);
        page->lock.Acquire();
        if(!page->free)
        {
            ReleaseLock(&page->lock);
            goto SELECT_PAGE;
        }

        selectedPageList->count--;
        LinkedListRemove(&page->listnode);

        page->lock.Release();

        while(m_order > order)
        {
            page = ExpandPage(page);
            m_order--;
        }

        page->free = false;
        return page;
    }

    void FreePages(uintptr_t addr);
    void FreePages(PageData* pf);

    void MarkPagesUsed(Range range)
    {
        uintptr_t addrStart = ALIGN_DOWN(range.start, ARCH_PAGE_SIZE);
        uintptr_t addrEnd   = ALIGN_UP(range.end, ARCH_PAGE_SIZE);

        uintptr_t current;
        PageData *page;

        while (current <= addrEnd)
        {
            page = GetPage(current);

            if(page == NULL)
                continue;

            page->lock.Acquire();

            page->free = false;
            LinkedListRemove(&page->listnode);

            page->lock.Release();

            current += ARCH_PAGE_SIZE;
        }
    }

    PageData* GetPage(uintptr_t addr)
    {
        PageData *page = (PageData *)(KERNEL_PHYSICAL_PAGES + (phys / ARCH_PAGE_SIZE * sizeof(PageData)));
        if(IsPagePresent(GetKernelPages(), (uintptr_t) page))
            return page;
        else
            return NULL;
    }

    /**
     * @brief split an upper page into 2 small pages
     *
     *
     * @param node node that the page belongs to
     * @param page the page that need to be expanded
     * @return buddy_page_t* new page (Not in the list)
     */
    PageData* ExpandPage(PageData* page)
    {
        /* check whether it's possible to split it */
        if(!page->order)
            return NULL;

        page->lock.Acquire();

        /* Remove this page from upper order list */
        LinkedListRemove(&page->listnode);
        /* Decrease the order and find the lower tier list */
        PageList *freelist = &pageList[--page->order];
        
        /* Find another page descriptor and initialize it */
        pagedata_t *newPage = (pagedata_t *)(((uintptr_t) page) + ((1 << page->order) * sizeof(PageData)));

        newPage->order = page->order;
        newPage->addr = page->addr + ((1 << page->order) * ARCH_PAGE_SIZE);
        newPage->free = true;

        /* Insert this page into the lower tier free list */
        freelist->Add(newPage);

        page->lock.Release();

        return page;
    }

    /**
     * @brief combine adjacent pages to a larger page
     *
     * @param node
     * @param page
     * @return buddy_page_t* pointer to new page
     */
    PageData* CombinePage(PageData *page)
    {
        uint32_t orderSize = (1 << page->order) * ARCH_PAGE_SIZE;
        bool isAligned = !(page->addr % orderSize);

        pagedata_t *newPage = (pagedata_t *)(isAligned ? page->addr + orderSize : page->addr - orderSize);
        if(newPage->free)
        {
            LinkedListRemove(&newPage->listnode);
            PageData *result = isAligned ? page : newPage;
            pageList[++result->order].Add(result);
            return result;
        }
        else
        {
            pageList[page->order].Add(page);
            return NULL;
        }
    }
    void CombinePages(PageData *lpage, PageData *rpage);
} // namespace Memory

extern "C" uint64_t kallocpg(size_t amount)
{
    amount = ALIGN_PAGE(amount);
    Memory::PageData *physicalPage = Memory::AllocatePages(PAGE_ORDER(amount));
    if(physicalPage == NULL)
    {
        // Panic
        return 0;
    }

    uintptr_t physAddress = physicalPage->addr;
    uintptr_t virtAddress = Memory::
    Memory::KernelMapVirtualPages(physAddress, virtAddress, amount, PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE);
    Proc::GetKernlProcess()->m_Pages += amount;
}

extern "C" void kfreepg(uint64_t addr)
{

}

extern "C" uint64_t allocpg(size_t amount)
{

}

extern "C" void freepg(uintptr_t addr)
{

}