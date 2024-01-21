#include <siberix/mem/page.h>
#include <xtra-math/align.h>

#define BUDDY_BOUND_UPPER 10
#define BUDDY_BOUND_LOWER 0

namespace Kern::Mem {

    static inline bool buddyIsAligned(uint64_t address, uint8_t order)
    {
        return !(address % ((1 << order) * PAGE_SIZE_4K));
    }

    static inline bool buddyIsAligned(Page4K* page)
    {
        return buddyIsAligned(page->_address, page->_order);
    }

    static inline uint64_t buddyOffset(uint8_t order)
    {
        return ((1 << order) * PAGE_SIZE_4K);
    }

    static inline uint64_t buddyFixSize(uint64_t size)
    {
        return alignTwoExponent(size);
    }
}