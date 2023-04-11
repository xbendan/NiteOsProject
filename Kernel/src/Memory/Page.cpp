#include <Mem/Page.h>
#include <Mem/MemZone.h>
#include <Proc/Scheduler.h>

#ifdef ARCH_X86_64
#include <Arch/x86_64/MMU.h>
#endif

namespace Memory {
    BuddyZone zones[3];

    void BuddyInit() {
        BuddyZone *zone = &(zones[ZONE_NORMAL]);

        g_Blocks.ForEach([&](ValueRange &vals, int index) -> void
        {
            uint64_t addrStart = ALIGN_UP(vals.Start, PAGE_MAX_SIZE);
            uint64_t addrEnd = ALIGN_DOWN(vals.End, PAGE_MAX_SIZE);

            uint64_t current = addrStart;
            while (current < addrEnd - PAGE_MAX_SIZE && addrEnd > PAGE_MAX_SIZE)
            {
                page_t *pages = pageOf(current);
                if (pages)
                {
                    memset(pages, 0, sizeof(page_t) * PAGE_AMOUNT_PER_BLOCK);
                    for (size_t pageIndex = 0; pageIndex < PAGE_AMOUNT_PER_BLOCK; pageIndex++)
                    {
                        pages[pageIndex].flags = PFLAGS_FREE;
                        pages[pageIndex].first = pages;
                        pages[pageIndex].addr = current + (pageIndex * ARCH_PAGE_SIZE);
                    }
                    pages->order = PAGE_MAX_ORDER;

                    zone->pageList[PAGE_MAX_ORDER].Add(reinterpret_cast<ListNode<page_t> *>(pages));
                }
                current += PAGE_MAX_SIZE;
            }
        });
    }

    page_t *AllocatePhysMemory4KOrdered(uint8_t order) {
        if (order > PAGE_MAX_ORDER || order < PAGE_MIN_ORDER) {
            System::Panic("Invalid page order %u.", order);
            return nullptr;
        }
        
        BuddyZone *zone = &(zones[ZONE_NORMAL]);
        page_t *page;
        uint8_t orderCopy = order;

        LinkedList<page_t> *list = nullptr;
        while (orderCopy <= PAGE_MAX_ORDER) {
            if(zone->pageList[orderCopy].Count()) {
                list = &(zone->pageList[orderCopy]);
                break;
            }
            orderCopy++;
        }

        if(list == nullptr) {
            return nullptr;
        }

        page = (page_t *) list->Extract();

        while (orderCopy-- > order) {
            page = ExpandPage(page);
        }
        
        page->flags &= ~PFLAGS_FREE;

        return page;
    }

    page_t *AllocatePhysMemory4K(size_t amount) {
        return AllocatePhysMemory4KOrdered(PAGE_ORDER(ALIGN_PAGE(amount)));
    }

    void FreePhysMemory4K(uintptr_t address) {

    }

    void FreePhysMemory4K(page_t *page) {

    }

    /**
     * @brief split an upper page into 2 small pages
     *
     *
     * @param node node that the page belongs to
     * @param page the page that need to be expanded
     * @return buddy_page_t* new page (Not in the list)
     */
    page_t* ExpandPage(page_t* page) {
        /* check whether it's possible to split it */
        if(!page || page->flags & PFLAGS_KMEM) {
            return nullptr;
        }
        // page->lock.Acquire();

        BuddyZone *zone = &(zones[ZONE_NORMAL]);
        /* Remove this page from upper order list */
        zone->pageList[page->order].Remove((ListNode<page_t> *) page);
        /* Decrease the order and find the lower tier list */
        LinkedList<page_t> *freelist = &(zone->pageList[--(page->order)]);
 
        /* Find another page descriptor and initialize it */
        page_t *newPage = reinterpret_cast<page_t *>(((uint64_t) page) + ((1 << page->order) * sizeof(page_t)));

        newPage->order = page->order;
        newPage->flags |= PFLAGS_FREE;

        /* Insert this page into the lower tier free list */
        freelist->Add((ListNode<page_t> *) newPage);

        // page->lock.Release();

        return page;
    }

    /**
     * @brief combine adjacent pages to a larger page
     *
     * @param node
     * @param page
     * @return page_t* pointer to new page
     */
    page_t* CombinePage(page_t *page) {
        uint32_t orderSize = (1 << (page->order)) * sizeof(page_t);
        bool align = !(page->addr % orderSize);
        BuddyZone *zone = &(zones[ZONE_NORMAL]);

        page_t *newPage = reinterpret_cast<page_t *>(align ? page + orderSize : page - orderSize);
        if (newPage->flags & PFLAGS_FREE) {
            page_t *result = align ? page : newPage;
            zone->pageList[newPage->order].Remove((ListNode<page_t> *) newPage);
            zone->pageList[++result->order].Add((ListNode<page_t> *) result);

            return result;
        } else {
            zone->pageList[page->order].Add((ListNode<page_t> *) page);
            return nullptr;
        }
    }

    void CombinePage(page_t *lpage, page_t *rpage) {

    }

    uintptr_t KernelAllocate4KPages(size_t amount) { Task::g_KernelProcess.m_AddressSpace->Allocate4KPages(amount); }
    uintptr_t KernelFree4KPages(uintptr_t address, size_t amount) { Task::g_KernelProcess.m_AddressSpace->Free4KPages(address, amount); }
} // namespace Memory
