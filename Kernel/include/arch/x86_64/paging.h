#pragma once

#include <arch/x86_64/kaddr.h>
#include <common/string.h>
#include <common/typedefs.h>
#include <siberix/mm/addrspace.h>
#include <siberix/mm/page.h>

#define PAGES_PER_TABLE 512
#define TABLES_PER_DIR 512
#define DIRS_PER_PDPT 512
#define PDPTS_PER_PML4 512
#define PML4_GET_INDEX(addr) (((addr) >> 39) & 0x1FF)
#define PDPT_GET_INDEX(addr) (((addr) >> 30) & 0x1FF)
#define PDIR_GET_INDEX(addr) (((addr) >> 21) & 0x1FF)
#define PAGE_GET_INDEX(addr) (((addr) >> 12) & 0x1FF)
#define PAGE_FRAME 0xFFFFFFFFF000ULL
#define KERNEL_HEAP_PML4_INDEX 511
#define KERNEL_HEAP_PDPT_INDEX 511

namespace Paging {

typedef u64 pml4e_t;
typedef u64 pdpte_t;
typedef u64 pde_t;
typedef u64 page_t;

using pml4_t      = pml4e_t[PDPTS_PER_PML4]; /* 512GiB -> 256TiB */
using pdpt_t      = pdpte_t[DIRS_PER_PDPT];  /* 1GiB -> 512GiB */
using pagedir_t   = pde_t[TABLES_PER_DIR];   /* 2MiB -> 1GiB */
using pagetable_t = page_t[PAGES_PER_TABLE]; /* 4KiB -> 2MiB */

enum PageFlags
{
    Present       = 0x01,
    Writable      = 0x02,
    User          = 0x04,
    WriteThrough  = 0x08,
    CacheDisabled = 0x10,
    DirectAddress = 0x80
};

inline constexpr void
setPageFlags(u64* page, u64 flags)
{
    *page |= flags;
}

inline constexpr void
setPageAddress(u64* page, u64 addr)
{
    *page = (*page & ~PAGE_FRAME) | (addr & PAGE_FRAME);
}

inline constexpr void
setPageFrame(u64* page, u64 addr, u64 flags)
{
    *page = (*page & ~PAGE_FRAME) | (addr & PAGE_FRAME) | flags;
}

class X64AddressSpace : public AddressSpace
{
public:
    pml4_t   pml4 __attribute__((aligned(PAGE_SIZE_4K)));
    pdpt_t   pdpts __attribute__((aligned(PAGE_SIZE_4K)));
    pde_t*   pageDirs[DIRS_PER_PDPT] __attribute__((aligned(PAGE_SIZE_4K)));
    page_t** pageTables[DIRS_PER_PDPT] __attribute__((aligned(PAGE_SIZE_4K)));
    /*
     * Each PDPT contains 1024 * 1024 / 4 = 262144 pages
     * 1 byte = 8 bits, 262144 pages needs 32768 bytes to manage
     *
     * 3 Level Bitmap
     * - 512 pointers to next level pointers, point at a 4K page. PDPT 1GB
     * for each
     * - A 4K page contains 512 pointers to bits. PDIR 2MB for each
     * - 512 bits total, 32 bytes
     */
    u64      pml4Phys;
    pdpt_t*  kernelPdpts;

    X64AddressSpace() {}
    ~X64AddressSpace() {}

    u64  allocate4KPages(u64 amount) override;
    void free4KPages(u64 address, u64 amount) override;
    void map(u64 phys, u64 virt, u64 amount) override;
    bool isPagePresent(u64 address) override;
    u64  convertVirtToPhys(u64 address) override;
    void apply() override { asm("mov %%rax, %%cr3" ::"a"(pml4Phys)); }
} __attribute__((packed));

class X64KernelAddressSpace : public X64AddressSpace
{
public:
    X64KernelAddressSpace()  = default;
    ~X64KernelAddressSpace() = default;

    u64  allocate4KPages(u64 amount) override;
    void free4KPages(u64 address, u64 amount) override;
    void map(u64 phys, u64 virt, u64 amount) override;
    bool isPagePresent(u64 address) override;
    u64  convertVirtToPhys(u64 address) override;
};

void
initializeVirtualMemory();

u64
kAllocate4KPages(u64 amount);

void
kFree4KPages(u64 address, u64 amount);

void
kMap4KPages(u64 phys, u64 virt, u64 amount);

} // namespace Paging