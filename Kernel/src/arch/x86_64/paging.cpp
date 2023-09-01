#include <arch/x86_64/iopt.h>
#include <arch/x86_64/kaddr.h>
#include <arch/x86_64/paging.h>
#include <common/logger.h>
#include <siberix/core/runtimes.h>

#define KERNEL_HEAP_PML4_INDEX 511
#define KERNEL_HEAP_PDPT_INDEX 511

using namespace Paging;

static pagedir_t    kPageDirs __attribute__((aligned(PAGE_SIZE_4K)));
static pagedir_t    kHeapDirs __attribute__((aligned(PAGE_SIZE_4K)));
static pagedir_t    kIoDirs[4] __attribute__((aligned(PAGE_SIZE_4K)));
static pagetable_t  kHeapTables[TABLES_PER_DIR] __attribute__((aligned(PAGE_SIZE_4K)));
static pagetable_t* kPageTablePointers[DIRS_PER_PDPT][TABLES_PER_DIR];

X64AddressSpace::X64AddressSpace() {}

X64AddressSpace::~X64AddressSpace() {}

u64 X64AddressSpace::allocate4KPages(u64 amount) {
    if (!amount) {
        Logger::getLogger("vmm").warn("Allocating 0 page, zero address returned.");
        return 0;
    }

    // X64KernelAddressSpace kspace;
    // u64                   offset        = 0;
    // u64                   pageDirOffset = 0;
    // u64                   counter       = 0;
    // u64                   address;

    return 0x0;
}

void X64AddressSpace::free4KPages(u64 address, u64 amount) {
    u64 pdptIndex, pdirIndex, pageIndex;
    while (amount) {
        pdptIndex = PDPT_GET_INDEX(address);
        pdirIndex = PDIR_GET_INDEX(address);
        if (!(pageDirs[pdptIndex][pdirIndex] & PageFlags::DirectAddress)) {
            pageIndex                                        = PAGE_GET_INDEX(address);
            m_bitmaps[pdptIndex][pdirIndex][pageIndex / 64] &= ~(1 << (pageIndex % 64));

            amount--;
        } else {
            for (int i = 0; i < 64; i++) {
                m_bitmaps[pdptIndex][pdirIndex][pageIndex / 64] = 0;
                pageDirs[pdptIndex][pdirIndex]                  = 0;

                amount -= PAGES_PER_TABLE;
            }
        }
    }
}

void X64AddressSpace::map(u64 phys, u64 virt, u64 amount) {}

bool X64AddressSpace::isPagePresent(u64 address) { return false; }

u64 X64AddressSpace::convertVirtToPhys(u64 address) { return 0; }

X64KernelAddressSpace::X64KernelAddressSpace() {
    // Initialize memory space for PML4 and PDPT
    memset(&pml4, 0, sizeof(pml4_t));
    memset(&pdpts, 0, sizeof(pdpt_t));

    // Set kernel PML4 entries
    setPageFrame(&(pml4[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)]),
                 ((u64)&pdpts) - KERNEL_VIRTUAL_BASE,
                 (PageFlags::Present | PageFlags::Writable));
    pml4[0] = pml4[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)];

    // Set kernel PDPT entries
    setPageFrame(&(pdpts[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)]),
                 ((u64)&kPageDirs) - KERNEL_VIRTUAL_BASE,
                 (PageFlags::Present | PageFlags::Writable));
    pageDirs[0] = &kPageDirs[0];
    for (int i = 0; i < TABLES_PER_DIR; i++) {
        setPageFrame(&(kPageDirs[i]),
                     (PAGE_SIZE_2M * i),
                     (PageFlags::Present | PageFlags::Writable | PageFlags::CacheDisabled |
                      PageFlags::DirectAddress));
    }

    // Set kernel heap space
    setPageFrame(&(pdpts[KERNEL_HEAP_PDPT_INDEX]),
                 ((u64)&kHeapDirs) - KERNEL_VIRTUAL_BASE,
                 (PageFlags::Present | PageFlags::Writable));
    for (int i = 0; i < TABLES_PER_DIR; i++) {
        setPageFrame(&(kHeapDirs[i]),
                     ((u64)&kHeapTables[i]) - KERNEL_VIRTUAL_BASE,
                     (PageFlags::Present | PageFlags::Writable));
        kPageTablePointers[KERNEL_HEAP_PDPT_INDEX][i] = &kHeapTables[i];
    }

    // Set kernel IO mapping
    for (int i = 0; i < 4; i++) {
        setPageFrame(&(pdpts[PDPT_GET_INDEX(IO_VIRTUAL_BASE) + i]),
                     ((u64)&kIoDirs[i] - KERNEL_VIRTUAL_BASE),
                     (PageFlags::Present | PageFlags::Writable));
        for (int j = 0; j < TABLES_PER_DIR; j++) {
            setPageFrame(&(kIoDirs[i][j]),
                         (PAGE_SIZE_1G * i + PAGE_SIZE_2M * j),
                         (PageFlags::Present | PageFlags::Writable | PageFlags::DirectAddress |
                          PageFlags::CacheDisabled));
        }
    }
    pdpts[0] = pdpts[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)];
    pml4Phys = ((u64)&pml4) - KERNEL_VIRTUAL_BASE;
    asm("mov %%rax, %%cr3" ::"a"(pml4Phys));
}

X64KernelAddressSpace::~X64KernelAddressSpace() {}

#include <siberix/display/types/vga.h>
extern VgaTextOutput _vga;

u64 X64KernelAddressSpace::allocate4KPages(u64 amount) {
    u64 offset        = 0;
    u64 pageDirOffset = 0;
    u64 counter       = 0;
    u64 address;

    _vga.drawText({ 0, 0 }, "Allocating 4K pages", Color(VgaTextColor::White));

    for (;;) asm("cli; hlt");

    for (int i = 0; i < TABLES_PER_DIR; i++) {
        for (int j = 0; j < PAGES_PER_TABLE; j++) {
            if (kHeapTables[i][j] & PageFlags::Present) {
                pageDirOffset = i;
                offset        = j + 1;
                counter       = 0;
                continue;
            }

            counter++;
            if (counter >= amount) {
                u64 physMemory = siberix()->getMemory().allocPhysMemory4K(amount);
                address =
                    ((pageDirOffset * PAGE_SIZE_2M) + (offset * PAGE_SIZE_4K)) | 0xFFFFFFFFC0000000;
                while (counter--) {
                    if (offset >= PAGES_PER_TABLE) {
                        pageDirOffset++;
                        offset = 0;
                    }

                    setPageFrame(
                        &(kHeapTables[pageDirOffset][offset]),
                        (physMemory + ((pageDirOffset * PAGES_PER_TABLE) + offset) * PAGE_SIZE_4K),
                        (PageFlags::Present | PageFlags::Writable));
                    offset++;
                }
                m_allocatedPages += amount;
                m_mappedPages    += amount;

                return address;
            }
        }
    }

    Logger::getLogger("vmm").warn("Failed to allocate %u pages", amount);
    return 0x0;
}

void X64KernelAddressSpace::free4KPages(u64 address, u64 amount) {}

void X64KernelAddressSpace::map(u64 phys, u64 virt, u64 amount) {}

bool X64KernelAddressSpace::isPagePresent(u64 address) {
    if (PML4_GET_INDEX(address) != KERNEL_HEAP_PML4_INDEX) {
        return false;
    }

    pagetable_t* table = kPageTablePointers[PDPT_GET_INDEX(address)][PDIR_GET_INDEX(address)];
    if (table != nullptr) {
        return (*table)[PAGE_GET_INDEX(address)] & PageFlags::Present;
    } else {
        return false;
    }
}

u64 X64KernelAddressSpace::convertVirtToPhys(u64 address) {
    if (PML4_GET_INDEX(address) != (PDPTS_PER_PML4 - 1)) {
        return 0;
    }

    u64 pdptIndex = PDPT_GET_INDEX(address);
    u64 pdirIndex = PDIR_GET_INDEX(address);
    u64 pageIndex = PAGE_GET_INDEX(address);
    if (pdptIndex != PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)) {
        return (PAGE_SIZE_2M * pdirIndex) + (address % PAGE_SIZE_2M);
    } else {
        pagetable_t* pageTable = kPageTablePointers[pdptIndex][pdirIndex];
        return (pageTable && ((*pageTable)[pageIndex] & PageFlags::Present))
                   ? (((*pageTable)[pageIndex] & PAGE_FRAME) + (address % PAGE_SIZE_4K))
                   : 0;
    }
}