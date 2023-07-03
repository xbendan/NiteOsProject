#include <common/logger.h>
#include <siberix/mm/page.hpp>
#include <siberix/mm/service.hpp>
#include <utils/alignment.h>

namespace Memory
{
    BuddyAlloc::BuddyAlloc(MemoryService& service) {
        SizedArrayList<PageBlock, 256>& pageBlocks = service.getPageBlocks();
        for (int i = 0; i < 256; i++)
        {
            PageBlock& block = pageBlocks[i];
            u64 addrStart = alignUp(block.start, PAGE_MAX_SIZE;);
            u64 addrEnd = alignDown(block.end, PAGE_MAX_SIZE);

            u64 current = addrStart;
            while (current < addrEnd - PAGE_MAX_SIZE)
            {
                Pageframe* pages = service.addr2page(current);
                if (!pages)
                {
                    Logger::getAnonymousLogger().error("Section pages is not allocated!");
                    return;
                }

                memset(pages, 0, sizeof(Pageframe) * PAGES_PER_SET);
                pages->order = PAGE_MAX_ORDER;
                for (u64 i = 0; i < PAGES_PER_SET; i++)
                {
                    pages[i].flags = 1;
                    pages[i].first = pages;
                    pages[i].address = current + (i * PAGE_SIZE_4K);
                }
                pageList[PAGE_MAX_ORDER].add(reinterpret_cast<ListNode<Pageframe>*> pages);

                current += PAGE_MAX_SIZE;
            }
        }
    }

    BuddyAlloc::~BuddyAlloc() {}

    Pageframe* BuddyAlloc::allocatePhysMemory4K(u64 amount) {}

    void BuddyAlloc::freePhysMemory4K(u64 address) {}

    void BuddyAlloc::freePhysMemory4K(Pageframe& pageframe) {}

    void BuddyAlloc::markPagesUsed(u64 addressStart, u64 addressEnd) {}

    Pageframe* BuddyAlloc::expand(Pageframe& pageframe) {}

    Pageframe* BuddyAlloc::combine(Pageframe& pageframe) {}

    Pageframe* BuddyAlloc::combine(Pageframe& lpage, Pageframe& rpage) {}
} // namespace Memory