#include <siberix/mem/buddy.h>
#include <siberix/mem/page.h>

namespace Kern::Mem {
    Page4K* Page4K::split()
    {
        if (!(_flags & PAGE_FLAG_FREE)) {
            return nullptr;
        }

        uint64_t offset   = ((1 << --_order) * sizeof(Page4K));
        Page4K*  newPage  = reinterpret_cast<Page4K*>(this + offset);
        newPage->_order   = _order;
        newPage->_flags   = 0;
        newPage->_address = _address + offset * PAGE_SIZE_4K;

        return newPage;
    }

    Page4K* Page4K::merge(Page4K* page)
    {
        if (/* Ensure both pages are free and can be merged */
            !(_flags & PAGE_FLAG_FREE && page->_flags & PAGE_FLAG_FREE) //
            ||
            /* Ensure these pages are connected in address */
            (abs(_address - page->_address) != (size()))) {

            return nullptr;
        }

        Page4K* result = (_address % (size() * 2)) //
                           ? this
                           : page;
        result->_order++;

        return result;
    }

    Page4K* Page4K::merge()
    {
        uint64_t offset = size();
        if (!buddyIsAligned(_address, _order + 1)) {
            offset = -offset;
        }

        Page4K* page = reinterpret_cast<Page4K*>(_address + offset);
        return merge(page);
    }
}