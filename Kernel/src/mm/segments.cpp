#include <common/logger.h>
#include <siberix/core/runtimes.h>
#include <siberix/init/boot.h>
#include <siberix/mm/page.h>
#include <utils/alignment.h>

SegAlloc::SegAlloc(SizedArrayList<PageBlock, 256>& blockRefs)
  : m_blockRefs(blockRefs)
{
}

SegAlloc::~SegAlloc() {}

PageFrame*
SegAlloc::allocatePhysMemory4KPages(u64 amount)
{
    u64        address = allocatePhysMemory4K(amount);
    PageFrame* page    = addr2page(address);
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
        PageBlock& block = m_blockRefs[i];
        if (block.m_type == PageBlockType::Available &&
            (block.m_end - block.m_start) > (amount * PAGE_SIZE_4K)) {
            address        = block.m_start;
            block.m_start += (amount * PAGE_SIZE_4K);
            break;
        }
        i++;
    }
    return address;
}

void
SegAlloc::freePhysMemory4K(u64 address)
{
}

void
SegAlloc::freePhysMemory4K(PageFrame* page)
{
}