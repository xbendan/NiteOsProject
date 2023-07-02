#include <siberix/mm/types.h>
#include <siberix/mm/page.hpp>
#include <utils/array_list.h>

extern SizedArrayList<AddressSegment, 128> addressSegments;

class SegmentAlloc : public PageAlloc {
public:
    SegmentAlloc();
    ~SegmentAlloc();

    Pageframe *allocatePhysMemory4K(u64 amount) override;
    void freePhysMemory4K(u64 address) override;
    void freePhysMemory4K(Pageframe *page) override;
    void addSegment(u64 start, u64 end, AddressSegmentType type);

private:
    SizedArrayList<AddressSegment, 256> segments;
    
};