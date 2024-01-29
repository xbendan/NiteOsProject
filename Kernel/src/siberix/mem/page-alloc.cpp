#include <siberix/mem/mem-alloc.h>
#include <siberix/mem/page.h>

namespace Kern::Mem {

    BuddyAlloc::BuddyAlloc() {}

    BuddyAlloc::~BuddyAlloc() {}

    Page4K* BuddyAlloc::allocatePhysMemory4KPages(uint64_t amount)
    {
        if ((!amount) || (amount > (1 << BUDDY_BOUND_UPPER))) {
            return nullptr;
        }

        amount      = buddyFixSize(amount);
        uint8_t ord = 0;
        for (; ord < BUDDY_BOUND_UPPER; ord++)
            if ((1 << ord) >= amount && m_pageLists[ord].size())
                break;

        if (!m_pageLists[ord].size()) {
            return nullptr;
        }

        Page4K* page = m_pageLists[ord].takeFirst();
        while ((1 << ord) > amount) {
            ord--;
            m_pageLists[ord].add(*page->split());
        }

        page->_flags &= ~PAGE_FLAG_FREE;
        return page;
    }

    uint64_t BuddyAlloc::allocatePhysMemory4K(uint64_t amount)
    {
        return allocatePhysMemory4KPages(amount)->_address;
    }

    void BuddyAlloc::freePhysMemory4KPages(Page4K* page)
    {
        /* Ensure the page is okay to be free */
        if (page->_flags & (PAGE_FLAG_FREE | //
                            PAGE_FLAG_KMEM | //
                            PAGE_FLAG_SWAP)) {
            return;
        }

        /* Mark the page as free */
        page->_flags |= PAGE_FLAG_FREE;
        while (page->_order < BUDDY_BOUND_UPPER) {
            /*
                Merge the page until it reaches the upper bound
                or the another page needed is not free, then jump
                out of the loop
             */
            uint64_t newAddr = (page->_address % (1 << (page->size() * 2)))
                                 ? page->_address + page->size()
                                 : page->_address - page->size();
            Page4K*  newPage = pageOf(newAddr);

            if (newPage->_flags & PAGE_FLAG_FREE) {
                if (newPage->isLinked()) {
                    m_pageLists[newPage->_order].remove(*newPage);
                }

                Page4K* result = page->merge(newPage);
                if (result != nullptr) {
                    page = result;
                    continue;
                }
            } else
                break;
        }
        m_pageLists[page->_order].add(*page);
    }

    void BuddyAlloc::freePhysMemory4K(uint64_t address) {}
}