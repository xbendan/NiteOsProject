#include <common/arch.h>
#include <siberix/init/boot.h>
#include <siberix/mm/page.hpp>
#include <utils/alignment.h>

namespace Memory
{
    SegAlloc::SegAlloc() {
        BootConfig& bootConfig = getBootConfig();
        MemoryService& service = runtime()->getMemoryService();
        AddressSegment[256]& segments = bootConfig.memory.segments;
        u64 sectionPages = alignUp(alignUp(bootConfig.memory.maxSize, PAGE_SIZE_1G) / PAGE_SIZE_1G / 8, PAGE_SIZE_4K);

        for (int i = 0; i < 256; i++)
        {
            AddressSegment& seg = getSegments()[i];

            alignUpRef(seg.start, PAGE_SIZE_4K);
            alignDownRef(seg.end, PAGE_SIZE_4K);
            if (seg.type != AST_AVAILABLE || (seg.end - seg.start < PAGE_SIZE_4K)) { continue; }

            if (service.sections == nullptr && (seg.end - seg.start) / PAGE_SIZE_4K > sectionPages)
            {
                // allocate virtual memory
                service.sections = seg.start;
                seg.start += (sectionPages * PAGE_SIZE_4K);
            }

            u64 totalPageSize = PAGES_PER_SECTION * sizeof(Pageframe);
            u64 cur = seg.start;
            while (cur < seg.end)
            {
                PageSection* section = service.sectionAt(cur);
                if (!(section->pages)) {}
            }
        }
    }

    SegAlloc::~SegAlloc() {}

    Pageframe* SegAlloc::allocatePhysMemory4K(u64 amount) {}

    void SegAlloc::freePhysMemory4K(u64 address) {}

    void SegAlloc::freePhysMemory4K(Pageframe* page) {}

    void SegAlloc::addSegment(u64 start, u64 end, AddressSegmentType type) {
        this->segments.add(AddressSegment(start, end, type));
    }
} // namespace Memory
