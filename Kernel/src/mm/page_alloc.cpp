#include <common/logger.h>
#include <siberix/core/runtimes.h>
#include <siberix/mm/memory.h>
#include <siberix/mm/page.h>
#include <utils/alignment.h>

BuddyAlloc::BuddyAlloc() {
    for (int i = 0; i < 256; i++) {
        PageBlock& block     = siberix()->getMemory().getPageBlock(i);
        u64        addrStart = alignUp(block.start, PAGE_MAX_SIZE);
        u64        addrEnd   = alignDown(block.end, PAGE_MAX_SIZE);

        u64 address = addrStart;
        while (address < addrEnd - PAGE_MAX_SIZE) {
            Pageframe* pages = addr2page(address);
            if (!pages) {
                Logger::getAnonymousLogger().error("Section pages is not allocated!");
                return;
            }

            memset(pages, 0, sizeof(Pageframe) * PAGES_PER_SET);
            pages->order = PAGE_MAX_ORDER;
            for (u64 i = 0; i < PAGES_PER_SET; i++) {
                pages[i].flags   = 1;
                pages[i].first   = pages;
                pages[i].address = address + (i * PAGE_SIZE_4K);
            }
            pageList[PAGE_MAX_ORDER].add(reinterpret_cast<ListNode<Pageframe>*>(pages));

            address += PAGE_MAX_SIZE;
        }
    }
}

BuddyAlloc::~BuddyAlloc() {}

Pageframe* BuddyAlloc::allocatePhysMemory4KPages(u64 amount) {
    u8 order = getPageOrder(getPageAlignment(amount));
    if (order > PAGE_MAX_ORDER) {
        return nullptr;
    }

    Pageframe*             page;
    u8                     _order = order;
    LinkedList<Pageframe>* list;
    while (_order <= PAGE_MAX_ORDER) {
        if (pageList[_order].count()) {
            list = &(pageList[_order]);
            break;
        }
        _order++;
    }

    if (list != nullptr) {
        page = reinterpret_cast<Pageframe*>(list->extract());
        while (_order-- > order) {
            page = expand(page);
        }
        page->flags &= ~PFLAGS_FREE;
        return page;
    } else {
        Logger::getLogger("mem").error("Cannot find any page with specific size. Out of Memory!");
        return nullptr;
    }
}

u64 BuddyAlloc::allocatePhysMemory4K(u64 amount) {
    return allocatePhysMemory4KPages(amount)->address;
}

void BuddyAlloc::freePhysMemory4K(u64 address) {}

void BuddyAlloc::freePhysMemory4K(Pageframe* page) {}

void BuddyAlloc::markPagesUsed(u64 addressStart, u64 addressEnd) {}

Pageframe* BuddyAlloc::expand(Pageframe* page) {
    if (page->flags & PFLAGS_KMEM) {
        Logger::getLogger("mem").warn(
            "Unable to expand page because it belongs to kernel "
            "allocator.");
        return nullptr;
    }

    /* Remove this page from upper order list */
    pageList[page->order].remove(reinterpret_cast<ListNode<Pageframe>*>(page));
    /* Decrease the order and find the lower tier list */
    LinkedList<Pageframe>& freelist = pageList[--page->order];

    Pageframe* newPage =
        reinterpret_cast<Pageframe*>(((u64)&page) + ((1 << page->order) * sizeof(Pageframe)));

    newPage->order  = page->order;
    newPage->flags |= PFLAGS_FREE;

    /* Insert this page into the lower tier free list */
    freelist.add(reinterpret_cast<ListNode<Pageframe>*>(newPage));

    return page;
}

Pageframe* BuddyAlloc::combine(Pageframe* page) {
    u32  osize = (1 << (page->order)) * sizeof(Pageframe);
    bool align = !(page->address % osize);

    Pageframe* newPage = reinterpret_cast<Pageframe*>(align ? page + osize : page - osize);
    if (newPage->flags & PFLAGS_FREE) {
        Pageframe* result = align ? page : newPage;
        pageList[newPage->order].remove((ListNode<Pageframe>*)newPage);
        pageList[++result->order].add((ListNode<Pageframe>*)result);

        return result;
    } else {
        pageList[page->order].add((ListNode<Pageframe>*)page);
        return nullptr;
    }
}

Pageframe* BuddyAlloc::combine(Pageframe* lpage, Pageframe* rpage) {
    if (!(lpage->flags & PFLAGS_FREE) || !(rpage->flags & PFLAGS_FREE)) {
        return nullptr;
    }
    pageList[lpage->order].remove((ListNode<Pageframe>*)lpage);
    pageList[rpage->order].remove((ListNode<Pageframe>*)rpage);

    pageList[++lpage->order].add((ListNode<Pageframe>*)lpage);
    return lpage;
}