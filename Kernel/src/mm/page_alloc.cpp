#include <common/logger.h>
#include <siberix/core/runtimes.h>
#include <siberix/mm/memory.h>
#include <siberix/mm/page.h>
#include <utils/alignment.h>

BuddyAlloc::BuddyAlloc(SizedArrayList<PageSection, 256>& sectRefs,
                       SizedArrayList<PageBlock, 256>&   blockRefs)
{
    for (int i = 0; i < 256; i++) {
        PageBlock& block = blockRefs[i];
        if (block.m_type != PageBlockType::Available &&
            block.m_type != PageBlockType::Reclaimable) {
            continue;
        }

        u64 addrStart = alignUp(block.m_start, PAGE_MAX_SIZE);
        u64 addrEnd   = alignDown(block.m_end, PAGE_MAX_SIZE);

        if (!(addrStart && addrEnd)) {
            continue;
        }

        u64 address = addrStart;
        while (address < addrEnd - PAGE_MAX_SIZE) {
            u32 _sectId      = (address >> 30),
                _offset      = (address >> 12) - (_sectId * PAGES_PER_SET);
            PageFrame* pages = &(sectRefs[_sectId].pages[_offset]);
            if (!pages) {
                Logger::getAnonymousLogger().error(
                  "Section pages is not allocated!\n");
                return;
            }

            memset(pages, 0, sizeof(PageFrame) * PAGES_PER_SET);
            pages->order = PAGE_MAX_ORDER;
            for (u64 i = 0; i < PAGES_PER_SET; i++) {
                pages[i].flags   = 1;
                pages[i].first   = pages;
                pages[i].address = address + (i * PAGE_SIZE_4K);
            }
            pageList[PAGE_MAX_ORDER].add(
              reinterpret_cast<ListNode<PageFrame>*>(pages));

            address += PAGE_MAX_SIZE;
        }
    }
}

BuddyAlloc::~BuddyAlloc() {}

PageFrame*
BuddyAlloc::allocatePhysMemory4KPages(u64 amount)
{
    u8 order = getPageOrder(getPageAlignment(amount));
    if (order > PAGE_MAX_ORDER) {
        return nullptr;
    }

    PageFrame*             page   = nullptr;
    LinkedList<PageFrame>* list   = nullptr;
    u8                     _order = order;
    while (_order <= PAGE_MAX_ORDER) {
        if (pageList[_order].count()) {
            list = &(pageList[_order]);
            break;
        }
        _order++;
    }

    if (list != nullptr) {
        page = reinterpret_cast<PageFrame*>(list->extract());
        while (_order-- > order) {
            page = expand(page);
        }
        page->flags &= ~PFLAGS_FREE;
        return page;
    } else {
        Logger::getLogger("mem").error(
          "Cannot find any page with specific size. Out of Memory!");
        return nullptr;
    }
}

u64
BuddyAlloc::allocatePhysMemory4K(u64 amount)
{
    return allocatePhysMemory4KPages(amount)->address;
}

void
BuddyAlloc::freePhysMemory4K(u64 address)
{
}

void
BuddyAlloc::freePhysMemory4K(PageFrame* page)
{
}

void
BuddyAlloc::markPagesUsed(u64 addressStart, u64 addressEnd)
{
}

PageFrame*
BuddyAlloc::expand(PageFrame* page)
{
    if (page->flags & PFLAGS_KMEM) {
        Logger::getLogger("mem").warn(
          "Unable to expand page because it belongs to kernel "
          "allocator.");
        return nullptr;
    }

    /* Remove this page from upper order list */
    pageList[page->order].remove(reinterpret_cast<ListNode<PageFrame>*>(page));
    /* Decrease the order and find the lower tier list */
    LinkedList<PageFrame>& freelist = pageList[--page->order];

    u64        offset  = ((1 << page->order) * sizeof(PageFrame));
    PageFrame* newPage = reinterpret_cast<PageFrame*>(page + offset);

    newPage->order  = page->order;
    newPage->flags |= PFLAGS_FREE;

    /* Insert this page into the lower tier free list */
    freelist.add(reinterpret_cast<ListNode<PageFrame>*>(newPage));

    return page;
}

PageFrame*
BuddyAlloc::combine(PageFrame* page)
{
    u32  offset = (1 << (page->order)) * sizeof(PageFrame);
    bool align  = !(page->address % offset);

    PageFrame* newPage =
      reinterpret_cast<PageFrame*>(align ? page + offset : page - offset);
    if (newPage->flags & PFLAGS_FREE) {
        PageFrame* result = align ? page : newPage;
        pageList[newPage->order].remove((ListNode<PageFrame>*)newPage);
        pageList[++result->order].add((ListNode<PageFrame>*)result);

        return result;
    } else {
        pageList[page->order].add((ListNode<PageFrame>*)page);
        return nullptr;
    }
}

PageFrame*
BuddyAlloc::combine(PageFrame* lpage, PageFrame* rpage)
{
    if (!(lpage->flags & PFLAGS_FREE) || !(rpage->flags & PFLAGS_FREE)) {
        return nullptr;
    }
    pageList[lpage->order].remove((ListNode<PageFrame>*)lpage);
    pageList[rpage->order].remove((ListNode<PageFrame>*)rpage);

    pageList[++lpage->order].add((ListNode<PageFrame>*)lpage);
    return lpage;
}