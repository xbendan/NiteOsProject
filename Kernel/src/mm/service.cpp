#include <common/runtime.hpp>
#include <common/string.h>
#include <siberix/mm/service.hpp>

static SegAlloc _segmentAlloc;
static BuddyAlloc _buddyAlloc;

MemoryService::MemoryService() {
    this->pageAlloc = &(_segmentAlloc = Memory::SegAlloc());
    u64 maxSize = runtime()->getBootConfig().memory.maxSize;
    u64 current = 0;
    while (current < maxSize)
    {
        PageSection& section = getSectionAt(current);
        section.pages = alloc4KPages(SECTION_PAGE_SIZE / PAGE_SIZE_4K);
        current += PAGE_SIZE_1G;
    }
}

MemoryService::~MemoryService() {}

u64 MemoryService::alloc4KPages(u64 amount) { return alloc4KPages(amount, nullptr); }

u64 MemoryService::alloc4KPages(u64 amount, Pageframe** _pointer) {
    Pageframe* page = allocPhysMemory4KPages(amount);

    if (_pointer != nullptr) *_pointer = page;
    u64 phys = page->address;
    u64 virt = allocVirtMemory4KPages(amount);
    if (!(phys && virt)) }

void MemoryService::free4KPages(u64 address) {}

Pageframe* MemoryService::allocPhysMemory4KPages(u64 amount){return getPageAllocator()}

u64 MemoryService::allocPhysMemory4K(u64 amount) {}

void MemoryService::freePhysMemory4KPages(u64 address) {}

void MemoryService::freePhysMemory4KPages(Pageframe& page) {}

u64 MemoryService::allocVirtMemory4KPages(u64 amount) {}

void MemoryService::freeVirtMemory4KPages(u64 address) {}

u64 MemoryService::alloc(u64 size) {}

void MemoryService::free(u64 address) {}

Pageframe* MemoryService::pfn2page(u64 pfn) {}

u64 MemoryService::page2pfn(Pageframe& page) {}

void MemoryService::calculate() {}