#include <common/string.h>
#include <siberix/core/runtimes.h>
#include <siberix/mm/memory.h>
#include <siberix/proc/sched.h>

static SegAlloc   _segmentAlloc;
static BuddyAlloc _buddyAlloc;

MemoryService::MemoryService() {
    this->pageAlloc = &(_segmentAlloc = SegAlloc());
    u64 maxSize     = siberix()->getBootConfig().memory.maxSize;
    u64 current     = 0;
    while (current < maxSize) {
        PageSection& section  = getPageSect(current);
        section.pages         = (u64*)alloc4KPages(SECTION_PAGE_SIZE / PAGE_SIZE_4K);
        current              += PAGE_SIZE_1G;
    }
}

MemoryService::~MemoryService() {}

u64 MemoryService::alloc4KPages(u64 amount) { return alloc4KPages(amount, nullptr); }

u64 MemoryService::alloc4KPages(u64 amount, Pageframe** _pointer) {
    Pageframe* page = allocPhysMemory4KPages(amount);

    if (_pointer != nullptr) *_pointer = page;
    u64 phys = page->address;
    u64 virt = allocVirtMemory4KPages(amount);
    if (!(phys && virt)) {
        return 0;
    }
    thisProcess()->getAddressSpace()->map(virt, phys, amount);
    return virt;
}

void MemoryService::free4KPages(u64 address, u64 amount) {}

Pageframe* MemoryService::allocPhysMemory4KPages(u64 amount) {
    return pageAlloc->allocatePhysMemory4K(amount);
}

u64 MemoryService::allocPhysMemory4K(u64 amount) { return allocPhysMemory4KPages(amount)->address; }

void MemoryService::freePhysMemory4KPages(u64 address) {
    pageAlloc->freePhysMemory4K(addr2page(address));
}

void MemoryService::freePhysMemory4KPages(Pageframe* page) { pageAlloc->freePhysMemory4K(page); }

u64 MemoryService::allocVirtMemory4KPages(u64 amount) {
    return thisProcess()->getAddressSpace()->allocate4KPages(amount);
}

void MemoryService::freeVirtMemory4KPages(u64 address) {}

u64 MemoryService::alloc(u64 size) { return memoryAlloc->alloc(size); }

void MemoryService::free(u64 address) { memoryAlloc->free(address); }

void MemoryService::calculate() {}