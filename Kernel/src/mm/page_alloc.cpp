#include <siberix/mm/page.hpp>

namespace Memory
{
    Pageframe *BuddyAlloc::allocatePhysMemory4K(u64 amount) {

    }

    void BuddyAlloc::freePhysMemory4K(u64 address) {

    }

    void BuddyAlloc::freePhysMemory4K(Pageframe &pageframe) {

    }

    void BuddyAlloc::markPagesUsed(u64 addressStart, u64 addressEnd) {

    }

    Pageframe *BuddyAlloc::expand(Pageframe &pageframe) {

    }

    Pageframe *BuddyAlloc::combine(Pageframe &pageframe) {

    }

    Pageframe *BuddyAlloc::combine(Pageframe &lpage, Pageframe &rpage) {

    }
}