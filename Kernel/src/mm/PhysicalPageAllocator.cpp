#include <Init/BootInfo.h>
#include <Process/Scheduler.h>
#include <Memory/Page.h>
#include <mm/mem.h>
#include <Address>
#include <Kernel>

using namespace Utils;

namespace Memory
{
    LinkedList<PageFrame> pageList[PAGE_MAX_ORDER + 1];
    //PageList pageList[PAGE_MAX_ORDER + 1];

    static inline void InitPages(PageFrame *pages, uint64_t addr)
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
        pageList[PAGE_MAX_ORDER - 1].Add((ListNode<PageFrame> *) pages);
    }

    bool read_map_entry(MemoryMapEntry *mapEntry) {
        switch (mapEntry->m_Type) {
        case MemoryMapEntryTypeAvailable:
            memStats.available += 
        }
    }

    void init_block(uint64_t addrStart, uint64_t addrEnd) {
        uint64_t pointer = addrStart;
        /* Loop until all the addresses are initialized */
        while (pointer < addrEnd - PAGE_MAX_SIZE) {
            if(pointer < 0x100000)
                continue;
            /* 
             * Try to allocate 8 pages (order 4) for storing [struct pageframe]
             * 16 pages equal to 64 KiB, and one [struct pageframe] takes 64 bytes
             * 64 KiB = 65536 bytes (divide by 64 bytes is 1024)
             * which means that a space of 16 KiB allows us to store connected
             * 1024 [struct pageframe].
             * 
             * The maximum size of the page can describe is 1024 * 4 KiB = 4096 KiB = 4 MiB
             */
            page_t *pageSpace = AllocatePages(4);
            if (pageSpace != NULL) {
                uintptr_t pages = (pointer / ARCH_PAGE_SIZE * sizeof(pageframe_t)) + KERNEL_PHYSICAL_PAGES;
                KernelMapVirtualMemory4K(pageSpace->addr, pages, 16, PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE);
                InitPages((page_t *) pages, pointer);
            } else {
                /*
                 * Allocate virtual address and map it to physical address
                 * for storing and accessing [struct pageframe]
                 * 
                 * size equals to 16 * 4 pages = 65536 bytes = 1024 [struct pageframe]
                 */
                /*
                uintptr_t virt = mmu::kalloc4kpages(16);
                mmu::kmap(pointer, virt, 16, PAGE_FLAG_WRITABLE);
                init_pages((page_t *) virt, pointer);
                pallocpg(4);
                mmu::kmap(pointer, pages_virt(pointer), 16, PAGE_FLAG_WRITABLE);
                */

                uintptr_t virt = KernelAllocate4KPages(16);
                KernelMapVirtualMemory4K(pointer, virt, 16, PAGE_FLAG_WRITABLE);
                InitPages((page_t *) virt, pointer);
                AllocatePages(4);
                KernelMapVirtualMemory4K(pointer, (pointer / ARCH_PAGE_SIZE * sizeof(pageframe_t)) + KERNEL_PHYSICAL_PAGES, 16, PAGE_FLAG_WRITABLE);
            }
        }
    }

    void init_pgalloc() {
        int mapSize = bootInfo.m_Memory.m_MemoryMapSize;

        for (int index = 0; index < mapSize; index++) {
            MemoryMapEntry *mapEntry = &(bootInfo.m_Memory.m_MemoryMapEntries[mapIndex]);
            //size_t mapEntrySize = (mapEntry->m_AddrEnd - mapEntry->m_AddrStart);

            if(mapEntry->m_Type != MemoryMapEntryTypeAvailable) {
                return;
            }

            init_block(ALIGN_UP(mapEntry->m_AddrStart, ARCH_PAGE_SIZE), ALIGN_DOWN(mapEntry->m_AddrEnd, ARCH_PAGE_SIZE));          
        }
    }

    void LoadPageAllocator()
    {
        int mapSize = bootInfo.m_Memory.m_MemoryMapSize;
        uint64_t blockStartAddr;
        uint64_t blockEndAddr;

        for (int mapIndex = 0; mapIndex < mapSize; mapIndex++) {
            MemoryMapEntry *mapEntry = &(bootInfo.m_Memory.m_MemoryMapEntries[mapIndex]);
            size_t mapEntrySize = (mapEntry->m_AddrEnd - mapEntry->m_AddrStart);

            switch (mapEntry->m_Type)
            {
            case MemoryMapEntryTypeAvailable:
                memStats.available += mapEntrySize;
                break;
            case MemoryMapEntryTypeKernel:
                memStats.allocated += mapEntrySize;
                memStats.inuse += mapEntrySize;
                MarkPagesUsed((Range){
                    .start = ALIGN_DOWN(mapEntry->m_AddrStart, ARCH_PAGE_SIZE),
                    .end = ALIGN_UP(mapEntry->m_AddrEnd, ARCH_PAGE_SIZE)
                });
                continue;
            default:
                continue;
            }

            blockStartAddr = ALIGN_UP(mapEntry->m_AddrStart, PAGE_MAX_SIZE);
            blockEndAddr = ALIGN_DOWN(mapEntry->m_AddrEnd, PAGE_MAX_SIZE);

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
                    KernelMapVirtualMemory4K(
                        allocSpace->addr,
                        pages,
                        16,
                        PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE
                    );
                }
            }
        }

        asm("hlt");

        //MakeTripleFault();
    }

    PageFrame* AllocatePages(uint8_t order)
    {
        if(order > PAGE_MAX_ORDER || order < PAGE_MIN_ORDER)
            return NULL;

        PageFrame *page;
        uint8_t m_order = order;
        
        LinkedList<PageFrame> *pageListSelected = NULL;
        while(m_order <= PAGE_MAX_ORDER)
        {
            if(pageList[m_order].Count())
            {
                pageListSelected = &pageList[m_order];
                break;
            }
            m_order++;
        }
        asm("hlt");

        if(pageListSelected == NULL)
        {
            // panic! (no enough memory space.)
            // or try to swap pages, kill processes
            return NULL;
        }

    SELECT_PAGE:
        page = (PageFrame*)(pageListSelected->First());
        page->.Acquire();
        if(!page->free)
        {
            page->lock.Release();
            goto SELECT_PAGE;
        }

        pageListSelected->Remove((ListNode<PageFrame> *) page);

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
    void FreePages(PageFrame* pf);

    void MarkPagesUsed(Range range)
    {
        uintptr_t addrStart = ALIGN_DOWN(range.start, ARCH_PAGE_SIZE);
        uintptr_t addrEnd   = ALIGN_UP(range.end, ARCH_PAGE_SIZE);

        uintptr_t current;
        PageFrame *page;

        while (current <= addrEnd)
        {
            page = GetPage(current);

            if(page == NULL)
                continue;

            page->lock.Acquire();

            page->free = false;
            pageList[page->order].Remove((ListNode<PageFrame> *) page);

            page->lock.Release();

            current += ARCH_PAGE_SIZE;
        }
    }

    PageFrame* GetPage(uintptr_t addr)
    {
        PageFrame *page = (PageFrame *)(KERNEL_PHYSICAL_PAGES + (addr / ARCH_PAGE_SIZE * sizeof(PageFrame)));
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
    PageFrame* ExpandPage(PageFrame* page)
    {
        /* check whether it's possible to split it */
        if(!page->order)
            return NULL;

        page->lock.Acquire();

        /* Remove this page from upper order list */
        pageList[page->order].Remove((ListNode<PageFrame> *) page);
        /* Decrease the order and find the lower tier list */
        LinkedList<PageFrame> *freelist = &pageList[--page->order];
        
        /* Find another page descriptor and initialize it */
        pagedata_t *newPage = (pagedata_t *)(((uintptr_t) page) + ((1 << page->order) * sizeof(PageFrame)));

        newPage->order = page->order;
        newPage->addr = page->addr + ((1 << page->order) * ARCH_PAGE_SIZE);
        newPage->free = true;

        /* Insert this page into the lower tier free list */
        freelist->Add((ListNode<PageFrame> *)newPage);

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
    PageFrame* CombinePage(PageFrame *page)
    {
        uint32_t orderSize = (1 << page->order) * ARCH_PAGE_SIZE;
        bool isAligned = !(page->addr % orderSize);

        pagedata_t *newPage = (pagedata_t *)(isAligned ? page->addr + orderSize : page->addr - orderSize);
        if(newPage->free)
        {
            pageList[newPage->order].Remove((ListNode<PageFrame> *) newPage);
            PageFrame *result = isAligned ? page : newPage;
            pageList[++result->order].Add((ListNode<PageFrame> *) result);
            return result;
        }
        else
        {
            pageList[page->order].Add((ListNode<PageFrame> *) page);
            return NULL;
        }
    }
    void CombinePages(PageFrame *lpage, PageFrame *rpage);
} // namespace Memory

extern "C" uint64_t kallocpg(size_t amount)
{
    amount = ALIGN_PAGE(amount);
    Memory::PageFrame *physicalPage = Memory::AllocatePages(PAGE_ORDER(amount));
    if(physicalPage == NULL)
    {
        // Panic
        return 0;
    }

    uintptr_t physAddress = physicalPage->addr;
    uintptr_t virtAddress = Memory::KernelAllocate4KPages(amount);
    Memory::KernelMapVirtualMemory4K(physAddress, virtAddress, amount, PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE);
    Proc::GetKernelProcess()->m_Pages += amount;
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