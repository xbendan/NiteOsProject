#include <Mem/AddressSpace.h>
#include <Mem/Page.h>
#include <Proc/Scheduler.h>

#include <Arch/x86_64/MMU.h>

using namespace Paging;
using namespace Memory;

AddressSpace::AddressSpace()
  : m_AllocatedPages(0),
    m_MappedPages(0),
    m_VirtualPages(new Paging::VirtualPages())
{
    m_VirtualPages->kernelPdpts = &kernelPdpts;
}

AddressSpace::~AddressSpace()
{

}

uintptr_t AddressSpace::Allocate4KPages(size_t amount)
{
    if (!amount)
        return 0;

    VirtualPages *pagemap = reinterpret_cast<VirtualPages *>(m_VirtualPages);
    AddressSpace *kernelSpace = Task::g_KernelProcess.AddressSpace();
    uint64_t offset = 0, pageDirOffset = 0;
    uint64_t counter = 0;
    uintptr_t address;

    /* Each loop is 1G */
    for (int i = 0; i < DIRS_PER_PDPT; i++)
    {
        if (!pagemap->bitmaps[i])
        {
            PageFrame *page;
            pagemap->bitmaps[i] = kernelSpace->Allocate4KPages(1);
            pagemap->pageTables[i] = kernelSpace->Allocate4KPages(1);
            pagemap->pageDirs[i] = kernelSpace->Allocate4KPages(1);
            SetPageFrame(&pagemap->pdpts[i], page->address, (PagePresent | PageWritable | PageUser));
        }

        int j = 0;
        if (i == 0)
        {
            j = 5;
            pageDirOffset = 5;
        }
        for (; j < 512; j++)
        {
            if (counter >= amount)
            {
                address = (i * PAGE_SIZE_1G)
                    + (pageDirOffset * PAGE_SIZE_2M) 
                    + (offset * PAGE_SIZE_4K);
                while (counter)
                {
                    if ((counter >= PAGES_PER_TABLE) && !(offset % 512))
                    {
                        SetPageFrame(&pagemap->pageDirs[i][pageDirOffset], m_ZeroPage, (PagePresent | PageUser));
                        pageDirOffset++;
                        for (int index = 0; index < 32; index++)
                        {
                            pagemap->bitmaps[i][pageDirOffset][index] = -1;
                        }

                        counter -= PAGES_PER_TABLE;
                    }
                    else
                    {
                        pagemap->CheckPageTable(i, pageDirOffset);

                        SetPageFrame(&pagemap->pageTables[i][pageDirOffset][offset], m_ZeroPage, (PagePresent | PageUser));
                        pagemap->bitmaps[i][pageDirOffset][offset / 8] &= (1 << (offset % 8));
                        offset++;

                        counter--;
                    }
                }
            }

            pagemap->CheckBitmap(i, j);
            for (int k = 0; k < PAGES_PER_TABLE;)
            {
                uint64_t bits = pagemap->bitmaps[i][j][k % 64];
                if (bits == -1)
                {
                    counter = 0;
                    continue;
                }

                if (bits)
                {
                    if (bits & (1 << (k % 64)))
                    {
                        pageDirOffset = j;
                        offset = k + 1;
                        counter = 0;
                        continue;
                    }

                    counter++;
                }
                else
                    counter += PAGES_PER_TABLE;
            }
        }
    }
}

void AddressSpace::Free4KPages(uintptr_t address, size_t amount)
{
    VirtualPages *pagemap = reinterpret_cast<VirtualPages *>(m_VirtualPages);
    uint64_t pdptIndex;
    uint64_t pdirIndex;
    uint64_t pageIndex;

    while (amount)
    {
        pdptIndex = PDPT_GET_INDEX(address);
        pdirIndex = PDIR_GET_INDEX(address);

        if (!(pagemap->pageDirs[pdptIndex][pdirIndex] & PageDirectAddress))
        {
            pageIndex = PAGE_GET_INDEX(address);
            pagemap->bitmaps[pdptIndex][pdirIndex][pageIndex / 64] &= ~(1 << (pageIndex % 64));
            
            amount--;
        }
        else
        {
            for (int i = 0; i < 64; i++)
            {
                pagemap->bitmaps[pdptIndex][pdirIndex][pageIndex / 64] = 0;
            }
            pagemap->pageDirs[pdptIndex][pdirIndex] = 0x0;

            amount -= PAGES_PER_TABLE;
        }
    }
}

using namespace Paging;

KernelAddressSpace::KernelAddressSpace()
  : m_VirtualPages(&g_KernelPagemap)
{
    VirtualPages *pagemap = reinterpret_cast<VirtualPages *>(m_VirtualPages);

    memset(&pagemap->pml4, 0, sizeof(pml4_t));
    memset(&pagemap->pdpt, 0, sizeof(pdpt_t));

    SetPageFrame(&(pagemap->pml4[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)]), 
        (uint64_t)(&pagemap->pdpt - KERNEL_VIRTUAL_BASE), 
        (PagePresent | PageWritable));
    pagemap->pml4[0] = pagemap->pml4[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)];

    SetPageFrame(&(pagemap->pdpt[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)]), 
        (uint64_t)(&kernelPageDirs - KERNEL_VIRTUAL_BASE), 
        (PagePresent | PageWritable));
    for (int i = 0; i < TABLES_PER_DIR; i++) {
        pagemap->pageDirs[i] = &kernelPageDirs[i];
        SetPageFrame(&kernelPageDirs[i], (PAGE_SIZE_2M * i), (PagePresent | PageWritable));
    }
    

    SetPageFrame(&(pagemap->pdpts[KERNEL_HEAP_PDPT_INDEX]), 
        (uint64_t)(&kernelHeapPageDirs - KERNEL_VIRTUAL_BASE), 
        (PagePresent | PageWritable));
    for (int i = 0; i < TABLES_PER_DIR; i++)
    {
        SetPageFrame(&(kernelHeapPageDirs[i]), 
            (uint64_t) & (kernelHeapPageTables[i]) - KERNEL_VIRTUAL_BASE, 
            (PagePresent | PageWritable));
        kernelPageTablePointers[KERNEL_HEAP_PDPT_INDEX][i] = &(kernelHeapPageTables[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        SetPageFrame(&(pagemap->pdpts[PDPT_GET_INDEX(IO_VIRTUAL_BASE) + i]),
            (uint64_t)&(ioMappings[i]) - KERNEL_VIRTUAL_BASE,
            (PagePresent | PageWritable));
        for (int j = 0; j < TABLES_PER_DIR; j++)
        {
            SetPageFrame(&(ioMappings[i][j]),
                         (PAGE_SIZE_1G * i + PAGE_SIZE_2M * j),
                         (PagePresent | PageWritable | PageDirectAddress | PageCacheDisabled));
        }
    }
    pagemap->pdpts[0] = pagemap->pdpts[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)];

    pagemap->pml4Phys = (&pagemap->pml4) - KERNEL_VIRTUAL_BASE;
}

uintptr_t KernelAddressSpace::Allocate4KPages(size_t amount)
{
    amount = ALIGN_PAGE(amount);

    uint64_t offset = 0, pageDirOffset = 0;
    uint64_t counter = 0;
    uintptr_t address;
    uintptr_t phys = Memory::AllocatePhysMemory4K(amount);
    
    for (int i = 0; i < TABLES_PER_DIR; i++) {
        for (int j = 0; j < PAGES_PER_TABLE; j++) {
            if (kernelHeapPageTables[i][j] & PagePresent) {
                pageDirOffset = i;
                offset = j + 1;
                counter = 0;
                continue;
            }

            counter++;
            if (counter >= amount) {
                adderss = ((pageDirOffset * PAGE_SIZE_2M) + (offset * PAGE_SIZE_4K)) | 0xFFFFFFFFC0000000;
                uint64_t totalPageOffset = 0;
                while (counter) {
                    if (offset >= PAGES_PER_TABLE) {
                        pageDirOffset++;
                        offset = 0;
                    }

                    SetPageFrame(&(kernelHeapPageTables[pageDirOffset][offset]), 
                        (phys + ((pageDirOffset * PAGES_PER_TABLE) + offset) * ARCH_PAGE_SIZE),
                        (PagePresent | PageWritable));
                    offset++;
                }
                m_AllocatedPages += amount;
                m_MappedPages += amount;

                return address;
            }
        }
    }
    return 0;
}

void KernelAddressSpace::Free4KPages(uintptr_t address, size_t amount)
{

}

bool KernelAddressSpace::IsPagePresent(uintptr_t address)
{
    size_t pml4Index = PML4_GET_INDEX(addr);
    size_t pdptIndex = PDPT_GET_INDEX(addr);
    size_t pdirIndex = PDIR_GET_INDEX(addr);
    size_t pageIndex = PAGE_GET_INDEX(addr);

    if (pml4Index != KERNEL_HEAP_PML4_INDEX)
        return false;

    pagetable_t *table = kernelPageTablePointers[pdptIndex][pdirIndex];
    if (table != nullptr) {
        return (*table)[pageIndex] & PagePresent;
    } else {
        return false;
    }
}

uintptr_t KernelAddressSpace::ConvertVirtToPhys(uintptr_t address)
{
    size_t pml4Index = PML4_GET_INDEX(addr);
    size_t pdptIndex = PDPT_GET_INDEX(addr);
    size_t pdirIndex = PDIR_GET_INDEX(addr);
    size_t pageIndex = PAGE_GET_INDEX(addr);

    if (pml4Index != (PDPTS_PER_PML4 - 1))
        return 0;

    if (pdptIndex == PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE))
        return (PAGE_SIZE_2M * pdirIndex) + (address % PAGE_SIZE_2M);
    else
    {
        pagetable_t *pageTable = kernelPageTablePointers[pdptIndex][pdirIndex];
        if (pageTable && ((*pageTable)[pageIndex] & PagePresent)) {
            return ((*pageTable)[pageIndex] & PAGE_FRAME + (address % ARCH_PAGE_SIZE));
        } else {
            return 0;
        }
    }
}