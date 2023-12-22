#include <siberix/mem/mem-alloc.h>
#include <siberix/mem/page.h>

namespace Kern::Mem {
    /*
        The page list contains pages of 2^order size. Like the pages in
        freePages[0] is 2^0 = 1 pages.
     */
    Std::LinkedList<Page4K> _pageLists[BUDDY_BOUND_UPPER + 1];

    static inline bool checkAlignment(Page4K* page)
    {
        return !((page->_address) % ((1 << page->_order) * PAGE_SIZE_4K));
    }

    template <typename T>
        requires Std::IsIntegral<T> or Std::IsSame<T, Page4K*>
    T AllocatePhysMemory4K(uint64_t amount)
    {
        if ((!amount) || (amount > (1 << BUDDY_BOUND_UPPER))) {
            return nullptr;
        }

        amount      = alignTwoExponent(amount);
        uint8_t ord = 0;
        for (; ord < BUDDY_BOUND_UPPER; ord++)
            if ((1 << ord) >= amount && _pageLists[ord].count())
                break;

        Page4K* page = _pageLists->get(0);
        while ((1 << ord) > amount) {
        }

        Page4K* page = nullptr;
        if (list != nullptr) {
            page = reinterpret_cast<Page4K*>(*list->get(0));
            list->remove(0);
            while (listOrder-- > order) {
                page = expand(page);
            }
            page->_flags &= ~BUDDY_PFLAGS_FREE;
            return Std::IsIntegral<T> ? page->_address : page;
        } else {
            return nullptr;
        }
    }

    template <typename T>
        requires Std::IsIntegral<T> or Std::IsSame<T, Page4K*>
    void FreePhysMemory4K(T waitForRelease)
    {
    }
}