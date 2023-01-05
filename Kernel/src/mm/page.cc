#include <mm/page.h>
#include <mm/mmzone.h>

#ifdef ARCH_X86_64
#include <Arch/x86_64/MMU.h>
#endif

using namespace Utils;

namespace Memory {
    buddyzone_t *zones;

    void BuddyInit() {
        zones = Model::MemblockAllocate(1);
        memset(zones, 0, ARCH_PAGE_SIZE);
        buddyzone_t *zone = &(zones[ZoneNormal]);

        int index = 0;
        while (Model::memblocks[index].m_Amount) {
            mem_block_t *block = &(Model::memblocks[index]);
            
            uint64_t addrStart = ALIGN_UP(block->m_PageNum * ARCH_PAGE_SIZE, PAGE_MAX_SIZE);
            uint64_t addrEnd = ALIGN_DOWN((block->m_PageNum + block->m_Amount) * ARCH_PAGE_SIZE, PAGE_MAX_SIZE);

            for (; addrStart < addrEnd - PAGE_MAX_SIZE; addrStart += PAGE_MAX_SIZE) {
                if(addrStart < LOWMEM_RESERVED) {
                    continue;
                }
                page_t *pages = Model::GetPage(addrStart);
                if(pages) {
                    for (size_t pageIndex = 0; pageIndex < PAGE_AMOUNT_PER_BLOCK; pageIndex++) {
                        pages[pageIndex].addr = addrStart + (pageIndex * ARCH_PAGE_SIZE);
                    }
                    pages->flags |= PAGE_MAX_ORDER;
                    zone->pageList[PAGE_MAX_ORDER - 1].Add((listnode_t<page_t> *) pages);
                }
            }
        }
    }

    page_t *AllocatePhysMemory4K(uint8_t order) {
        if (order > PAGE_MAX_ORDER || order < PAGE_MIN_ORDER) {
            return nullptr;
        }
        buddyzone_t *zone = &(zones[ZoneNormal]);
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

        if(pageList == nullptr) {
            return nullptr;
        }

        page = reinterpret_cast<page_t *>(pageList->Extract());

        while (orderCopy > order) {
            page = ExpandPage(page);
            orderCopy--;
        }
        
        page->flags &= ~PFLAGS_FREE;
        return page;
    }

    page_t *AllocatePhysMemory4K(size_t amount) {
        return AllocatePhysMemory4K(PAGE_ORDER(ALIGN_PAGE(amount)));
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
        if(!(page->flags & PFLAGS_ORDER)) {
            return nullptr;
        }
        page->lock.Acquire();

        buddyzone_t zone = &(zones[ZoneNormal]);
        /* Remove this page from upper order list */
        zone->pageList[page->flags & PFLAGS_ORDER].Remove((listnode_t<page_t> *) page);
        /* Decrease the order and find the lower tier list */
        LinkedList<page_t> *freelist = &(zone->pageList[--page->order]);

        /* Find another page descriptor and initialize it */
        page_t *newPage = reinterpret_cast<page_t *>(((uint64_t) page) + ((1 << page->order) * sizeof(page_t)));

        newPage->order = page->order;
        newPage->flags |= PFLAGS_FREE;

        /* Insert this page into the lower tier free list */
        freelist->Add((listnode_t<page_t> *) newPage);

        page->lock.Release();

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
        uint32_t orderSize = (1 << (page->flags & PFLAGS_ORDER)) * sizeof(page_t);
        bool align = !(page->addr % orderSize);
        buddyzone_t *zone = &(zones[ZoneNormal]);

        page_t *newPage = reinterpret_cast<page_t *>(align ? page + orderSize : page - orderSize);
        if (newPage->flags & PFLAGS_FREE) {
            page_t *result = align ? page : newPage;
            zone->pageList[newPage->flags & PFLAGS_ORDER].Remove((listnode_t<page_t> *) newPage);
            zone->pageList[++result->order].Add((listnode_t<page_t> *) result);

            return result;
        } else {
            zone->pageList[page->order].Add((listnode_t<page_t> *) page);
            return nullptr;
        }
    }

    void CombinePage(page_t *lpage, page_t *rpage) {

    }
} // namespace Memory
