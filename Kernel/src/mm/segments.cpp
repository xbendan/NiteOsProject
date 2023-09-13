#include <common/logger.h>
#include <siberix/core/runtimes.h>
#include <siberix/init/boot.h>
#include <siberix/mm/page.h>
#include <utils/alignment.h>

extern Process kernelProcess;

SegAlloc::SegAlloc() {}

SegAlloc::~SegAlloc() {}

Pageframe*
SegAlloc::allocatePhysMemory4KPages(u64 amount)
{
    u64        address = allocatePhysMemory4K(amount);
    Pageframe* page    = addr2page(address);
    if (!page->address) {
        page->address = address;
    }
    return page;
}

u64
SegAlloc::allocatePhysMemory4K(u64 amount)
{
    u64 address = 0;
    int i       = 0;
    while (!address && i < 256) {
        PageBlock& block = siberix()->getMemory().getPageBlock(i);
        if (block.type == PageBlockType::Available &&
            block.end - block.start > (amount * PAGE_SIZE_4K)) {
            address      = block.start;
            block.start += (amount * PAGE_SIZE_4K);
            break;
        }
        i++;
    }
    Logger::getAnonymousLogger().info("Address=%x.\n", address);
    return address;
}

void
SegAlloc::freePhysMemory4K(u64 address)
{
}

void
SegAlloc::freePhysMemory4K(Pageframe* page)
{
}