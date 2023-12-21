#include <siberix/mem/mem-alloc.h>
#include <siberix/mem/page.h>

namespace Kern::Mem {
    static inline Boolean checkAlignment(Page4K* page)
    {
        return !((page->_address) % ((1 << page->_order) * PAGE_SIZE_4K));
    }

    PagedAllocatorBase::PagedAllocatorBase() {}

    template <typename T>
        requires Std::IsIntegral<T> or Std::IsSame<T, Page4K*>
    T PagedAllocatorBase::allocPhysMemory4K(UInt64 amount)
    {
        if (!amount || amount > (1 << PAGE_MAX_ORDER))
            return nullptr;

        amount          = alignTwoExponent(amount);
        UInt8  order    = 10;
        UInt64 estimate = 1024;
        while (estimate > amount) {
            estimate >>= 1;
            order--;
        }

        Std::LinkedList<Page4K*>* list      = nullptr;
        UInt8                     listOrder = order;
        while (listOrder <= PAGE_MAX_ORDER) {
            if (m_freePages[listOrder].count()) {
                list = &(m_freePages[listOrder]);
                break;
            }
            listOrder++;
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
        requires Std::Integral<T> or Std::Same<T, Page4K*>
    void PagedAllocatorBase::freePhysMemory4K(T waitForRelease)
    {
    }
}