#include <siberix/core/runtimes.h>
#include <siberix/mm/page.h>

u64        page2pfn(Pageframe& page) { return page.address / PAGE_SIZE_4K; }
Pageframe* pfn2page(u64 pfn) {
    u32 sectionId = pfn >> 18;
    u32 offset    = pfn - (sectionId * 262144);
    return reinterpret_cast<Pageframe*>(exec()->getMemory().getPageSect(sectionId).pages[offset]);
};
Pageframe*   addr2page(u64 address) { return pfn2page(address >> 12); }
PageSection* addr2sect(u64 address) { return &(exec()->getMemory().getPageSect(address >> 30)); }