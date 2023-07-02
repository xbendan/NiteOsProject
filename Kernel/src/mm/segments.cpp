#include <siberix/mm/segments.hpp>

static SegmentAlloc segAlloc;
static SizedArrayList<AddressSegment, 128> addressSegments;

SegmentAlloc::SegmentAlloc() {

}

SegmentAlloc::~SegmentAlloc() {

}

SegmentAlloc::allocatePhysMemory4K() {
    
}
