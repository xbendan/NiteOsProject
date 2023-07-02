#include <siberix/mm/page.hpp>

namespace Memory {
    SegAlloc::SegAlloc() {
        getSegments()->
    }

    SegAlloc::~SegAlloc() {

    }

    Pageframe* SegAlloc::allocatePhysMemory4K(u64 amount) {
        
    }

    void SegAlloc::freePhysMemory4K(u64 address) {

    }

    void SegAlloc::freePhysMemory4K(Pageframe *page) {

    }

    void SegAlloc::addSegment(u64 start, u64 end, AddressSegmentType type) {
        this->segments.add(AddressSegment(start, end, type));
    }
}
