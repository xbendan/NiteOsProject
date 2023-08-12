#include <common/string.h>
#include <siberix/core/runtimes.h>
#include <siberix/mm/service.h>

static SegAlloc   _segmentAlloc;
static BuddyAlloc _buddyAlloc;

MemoryManagement::MemoryManagement() {
    this->pageAlloc = &(_segmentAlloc = Memory::SegAlloc());
    u64 maxSize     = exec()->getBootConfig().memory.maxSize;
    u64 current     = 0;
    while (current < maxSize) {
        PageSection& section  = getSectionAt(current);
        section.pages         = (u64*)alloc4KPages(SECTION_PAGE_SIZE / PAGE_SIZE_4K);
        current              += PAGE_SIZE_1G;
    }
}

MemoryManagement::~MemoryManagement() {}

u64 MemoryManagement::alloc4KPages(u64 amount) { return alloc4KPages(amount, nullptr); }

u64 MemoryManagement::alloc4KPages(u64 amount, Pageframe** _pointer) {
    Pageframe* page = allocPhysMemory4KPages(amount);

    if (_pointer != nullptr) *_pointer = page;
    u64 phys = page->address;
    u64 virt = allocVirtMemory4KPages(amount);
    if (!(phys && virt)) {
        return 0;
    }
}

void MemoryManagement::free4KPages(u64 address, u64 amount) {}

Pageframe* MemoryManagement::allocPhysMemory4KPages(u64 amount) {
    return pageAlloc->allocatePhysMemory4K(amount);
}

u64 MemoryManagement::allocPhysMemory4K(u64 amount) {}

void MemoryManagement::freePhysMemory4KPages(u64 address) {}

void MemoryManagement::freePhysMemory4KPages(Pageframe& page) {}

u64 MemoryManagement::allocVirtMemory4KPages(u64 amount) {}

void MemoryManagement::freeVirtMemory4KPages(u64 address) {}

u64 MemoryManagement::alloc(u64 size) {}

void MemoryManagement::free(u64 address) {}

Pageframe* MemoryManagement::pfn2page(u64 pfn) {}

u64 MemoryManagement::page2pfn(Pageframe& page) {}

void MemoryManagement::calculate() {}