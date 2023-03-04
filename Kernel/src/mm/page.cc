#include <mm/page.h>
#include <mm/mmzone.h>
#include <driver/video.h>
#include <kern.h>

#ifdef ARCH_X86_64
#include <Arch/x86_64/mmu.h>
#endif

using namespace Utils;

namespace Memory {
    buddyzone_t zones[3];

    void BuddyInit() {
        buddyzone_t *zone = &(zones[ZONE_NORMAL]);

        using namespace Model;

        int index = 0;
        mem_block_t *block;
        while (memblocks[index].m_AddrStart) {
            block = &memblocks[index];
            uint64_t addrStart = ALIGN_UP(block->m_AddrStart, PAGE_MAX_SIZE);
            uint64_t addrEnd = ALIGN_DOWN(block->m_AddrEnd, PAGE_MAX_SIZE);

            for (; addrStart < addrEnd - PAGE_MAX_SIZE; addrStart += PAGE_MAX_SIZE) {
                if(addrStart < LOWMEM_RESERVED || addrEnd < PAGE_MAX_ORDER) {
                    continue;
                }

                page_t *pages = Model::GetPage(addrStart);
                if (pages) {
                    memset(pages, 0, sizeof(page_t) * PAGE_AMOUNT_PER_BLOCK);
                    
                    for (size_t pageIndex = 0; pageIndex < PAGE_AMOUNT_PER_BLOCK; pageIndex++)
                    {
                        pages[pageIndex].flags = PFLAGS_FREE;
                        pages[pageIndex].first = pages;
                        pages[pageIndex].addr = addrStart + (pageIndex * ARCH_PAGE_SIZE);
                    }
                    
                    pages->order = PAGE_MAX_ORDER;
                    zone->pageList[PAGE_MAX_ORDER].Add((ListNode<page_t> *) pages);
                }
            }
            index++;
        }
    }

    page_t *AllocatePhysMemory4KOrdered(uint8_t order) {
        if (order > PAGE_MAX_ORDER || order < PAGE_MIN_ORDER) {
            System::Panic("Invalid page order %u.", order);
            return nullptr;
        }
        
        buddyzone_t *zone = &(zones[ZONE_NORMAL]);
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

        buddyzone_t *zone = &(zones[ZONE_NORMAL]);
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
        buddyzone_t *zone = &(zones[ZONE_NORMAL]);

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
} // namespace Memory
