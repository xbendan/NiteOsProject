#include <arch/x86_64/kaddr.hpp>
#include <arch/x86_64/paging.hpp>

#define KERNEL_HEAP_PML4_INDEX 511
#define KERNEL_HEAP_PDPT_INDEX 511

using namespace Paging;

X64AddressSpace::X64AddressSpace()
    : m_allocatedPages(0),
      m_mappedPages(0) {}

X64AddressSpace::~X64AddressSpace() {}

X64KernelAddressSpace::X64KernelAddressSpace()
    : m_allocatedPages(0),
      m_mappedPages(0),
      _zeroPage() {
    memset(&pml4, 0, sizeof(pml4_t));
    memset(&pdpts, 0, sizeof(pdpt_t));

    setPageFrame(&(pml4[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)]),
                 ((u64)&pdpts) - KERNEL_VIRTUAL_BASE,
                 (PageFlags::Present | PageFlags::Writable));
    pml4[0] = pml4[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)];

    setPageFrame(&(pdpts[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)]),
                 ((u64)&pageDirs) - KERNEL_VIRTUAL_BASE,
                 (PageFlags::Present | PageFlags::Writable));
    pageDirs[0] = &kPageDirs[0];
    for (int i = 0; i < TABLES_PER_DIR; i++) {
        setPageFrame(&(kPageDirs[i]),
                     (PAGE_SIZE_2M * i),
                     (PageFlags::Present | PageFlags::Writable |
                      PageFlags::CacheDisabled | PageFlags::DirectAddress));
    }

    setPageFrame(&(pdpts[KERNEL_HEAP_PDPT_INDEX]),
                 ((u64)&kHeapDirs) - KERNEL_VIRTUAL_BASE,
                 (PageFlags::Present | PageFlags::Writable));
    for (int i = 0; i < TABLES_PER_DIR; i++) {
        setPageFrame(&(pdpts[PDPT_GET_INDEX(IO_VIRTUAL_BASE) + i]),
                     ((u64)&kIoDirs[i]) - KERNEL_VIRTUAL_BASE),
            (PageFlags::Present | PageFlags::Writable);
        for (int j = 0; j < TABLES_PER_DIR; j++) {
            setPageFrame(&(kIoDirs[i][j]),
                         (PAGE_SIZE_1G * i + PAGE_SIZE_2M * j),
                         (PageFlags::Present | PageFlags::Writable |
                          PageFlags::DirectAddress | PageFlags::CacheDisabled));
        }
    }
    pdpts[0] = pdpts[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)];
    pml4Phys = ((u64)&pml4) - KERNEL_VIRTUAL_BASE;
}

X64KernelAddressSpace::~X64KernelAddressSpace() {}