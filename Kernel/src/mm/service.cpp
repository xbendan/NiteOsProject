#include <siberix/mm/service.hpp>

static SegAlloc _segmentAlloc;
static BuddyAlloc _buddyAlloc;

MemoryService::MemoryService() { this->pageAlloc = &(_segmentAlloc = Memory::SegAlloc()); }

MemoryService::~MemoryService() {}

u64 MemoryService::alloc4KPages(u64 amount) {}

void MemoryService::free4KPages(u64 address) {}

Pageframe* MemoryService::allocPhysMemory4KPages(u64 amount) {}

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