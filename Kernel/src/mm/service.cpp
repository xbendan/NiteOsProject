#include <common/string.h>
#include <siberix/core/runtimes.h>
#include <siberix/mm/addrspace.h>
#include <siberix/mm/memory.h>
#include <siberix/mm/slab.h>
#include <siberix/proc/sched.h>
#include <utils/alignment.h>

static SegAlloc   _segAlloc;
static BuddyAlloc _buddyAlloc;
static SlabAlloc  _slabAlloc;
extern Process    kernelProcess;

#include <arch/x86_64/iopt.h>
#include <siberix/display/types/vga.h>

MemoryController::MemoryController() {
    PageBlock* pageBlock = &(siberix()->getBootConfig().memory.ranges[0]);
    for (int i = 0; i < 256; i++) {
        alignUpRef(pageBlock->start, PAGE_SIZE_4K);
        alignDownRef(pageBlock->end, PAGE_SIZE_4K);
        if (pageBlock->end == 0) {
            break;
        }
        if ((pageBlock->end - pageBlock->start) < PAGE_SIZE_4K) {
            continue;
        }

        m_pageBlocks.add(*pageBlock);
    }

    this->m_pageAlloc             = &(_segAlloc = SegAlloc());
    AddressSpace* kernelAddrSpace = kernelProcess.getAddressSpace();

    Logger::getAnonymousLogger().info("Allocating kernel address space");

    const u64 amount = SECTION_PAGE_SIZE / PAGE_SIZE_4K;
    for (u64 address  = 0; address < siberix()->getBootConfig().memory.maxSize;
         address     += PAGE_SIZE_1G) {
        PageSection& section = getPageSect(address);
        if (!section.pages) {
            u64 virt = kernelAddrSpace->allocate4KPages(amount);
            // u64 phys      = allocPhysMemory4K(amount);
            // section.pages = reinterpret_cast<u64*>(virt);
            // kernelAddrSpace->map(virt, phys, amount);
        }
    }

    this->m_pageAlloc   = &(_buddyAlloc = BuddyAlloc());
    this->m_memoryAlloc = &(_slabAlloc = SlabAlloc());

    outWord16(0x604, 0x2000);
}

MemoryController::~MemoryController() {}

u64 MemoryController::alloc4KPages(u64 amount) { return alloc4KPages(amount, nullptr); }

u64 MemoryController::alloc4KPages(u64 amount, Pageframe** _pointer) {
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

void MemoryController::free4KPages(u64 address, u64 amount) {}

Pageframe* MemoryController::allocPhysMemory4KPages(u64 amount) {
    return m_pageAlloc->allocatePhysMemory4KPages(amount);
}

u64 MemoryController::allocPhysMemory4K(u64 amount) {
    return m_pageAlloc->allocatePhysMemory4K(amount);
}

void MemoryController::freePhysMemory4KPages(u64 address) {
    m_pageAlloc->freePhysMemory4K(addr2page(address));
}

void MemoryController::freePhysMemory4KPages(Pageframe* page) {
    m_pageAlloc->freePhysMemory4K(page);
}

u64 MemoryController::allocVirtMemory4KPages(u64 amount) {
    return thisProcess()->getAddressSpace()->allocate4KPages(amount);
}

void MemoryController::freeVirtMemory4KPages(u64 address) {}

u64 MemoryController::alloc(u64 size) { return m_memoryAlloc->alloc(size); }

void MemoryController::free(u64 address) { m_memoryAlloc->free(address); }

void MemoryController::calculate() {}