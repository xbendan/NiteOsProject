#pragma once

#include <macros>
#include <Mem/Memory.h>
#include <Mem/AddressSpace.h>
#include <System.h>

#define ARCH_PAGE_SIZE              (4096)
#define PAGES_PER_TABLE             512
#define TABLES_PER_DIR              512
#define DIRS_PER_PDPT               512
#define PDPTS_PER_PML4              512
#define PAGE_SIZE_4K                0x1000U
#define PAGE_SIZE_2M                0x200000U
#define PAGE_SIZE_1G                0x40000000U
#define PDPT_SIZE                   0x8000000000U
#define PML4_GET_INDEX(addr)        (((addr) >> 39) & 0x1FF)
#define PDPT_GET_INDEX(addr)        (((addr) >> 30) & 0x1FF)
#define PDIR_GET_INDEX(addr)        (((addr) >> 21) & 0x1FF)
#define PAGE_GET_INDEX(addr)        (((addr) >> 12) & 0x1FF)
#define PAGE_FRAME                  0xFFFFFFFFF000ULL
#define PAGE_PRESENT                (1 << 0)
#define PAGE_WRITABLE               (1 << 1)
#define PAGE_USER                   (1 << 2)
#define PAGE_WRITETHROUGH           (1 << 3)
#define PAGE_NOCACHE                (1 << 4)
#define PAGE_SIZE                   (1 << 7)
#define IS_PAGE_ALIGNED(addr)       (addr % ARCH_PAGE_SIZE == 0)

#define KERNEL_HEAP_PML4_INDEX 511
#define KERNEL_HEAP_PDPT_INDEX 511

namespace Paging
{  
    typedef uint64_t pml4e_t;
    typedef uint64_t pdpte_t;
    typedef uint64_t pde_t;
    typedef uint64_t page_t;
    typedef uint64_t page_flags_t;

    using pml4_t = pml4e_t[PDPTS_PER_PML4]; /* 512GiB -> 256TiB */
    using pdpt_t = pdpte_t[DIRS_PER_PDPT]; /* 1GiB -> 512GiB */
    using pagedir_t = pde_t[TABLES_PER_DIR]; /* 2MiB -> 1GiB */
    using pagetable_t = page_t[PAGES_PER_TABLE]; /* 4KiB -> 2MiB */

    extern pagedir_t   kernelPageDirs __attribute__((aligned(ARCH_PAGE_SIZE)));
    extern pagedir_t   kernelHeapPageDirs __attribute__((aligned(ARCH_PAGE_SIZE)));
    extern pagetable_t kernelHeapPageTables[TABLES_PER_DIR] __attribute__((aligned(ARCH_PAGE_SIZE)));
    extern pagedir_t   ioMappings[4] __attribute__((aligned(ARCH_PAGE_SIZE)));
    extern pagetable_t *kernelPageTablePointers[DIRS_PER_PDPT][TABLES_PER_DIR];

    enum PageFlags
    {
        PagePresent = 0x01,
        PageWritable = 0x02,
        PageUser = 0x04,
        PageWriteThrough = 0x08,
        PageCacheDisabled = 0x10,
        PageDirectAddress = 0x80
    };

    class VirtualPages
    {
    public:
        pml4_t pml4 __attribute__((aligned(ARCH_PAGE_SIZE)));
        pdpt_t pdpts __attribute__((aligned(ARCH_PAGE_SIZE)));
        /*
         * 
         */
        pde_t *pageDirs[DIRS_PER_PDPT] __attribute__((aligned(ARCH_PAGE_SIZE)));
        page_t **pageTables[DIRS_PER_PDPT] __attribute__((aligned(ARCH_PAGE_SIZE)));
        /*
         * Each PDPT contains 1024 * 1024 / 4 = 262144 pages
         * 1 byte = 8 bits, 262144 pages needs 32768 bytes to manage
         * 
         * 3 Level Bitmap
         * - 512 pointers to next level pointers, point at a 4K page. PDPT 1GB for each
         * - A 4K page contains 512 pointers to bits. PDIR 2MB for each
         * - 512 bits total, 32 bytes
         */
        uint64_t **bitmaps[DIRS_PER_PDPT];
        uint64_t pml4Phys;
        pdpt_t *kernelPdpts;

        void CheckBitmap(uint64_t d, uint64_t t);
        void CheckPageTable(uint64_t d, uint64_t t);
    };

    /**
     * @brief Initialize kernel page map and virtual memory management
     */
    void InitializeVirtualMemory(void);
    /**
     * @brief 
     * 
     * @param map 
     * @param addr 
     * @return true If the page is present (in the memory)
     * @return false If the page does not exist or be swapped into disks.
     */
    bool IsPagePresent(VirtualPages *pagemap, uint64_t addr);
    /**
     * @brief Create a Pagemap object
     * 
     * @return VirtualPages* 
     */
    VirtualPages *CreatePagemap();
    /**
     * @brief 
     * 
     */
    void DestoryPagemap();

    /**
     * @brief 
     * 
     * @param map 
     * @param phys 
     * @param virt 
     * @param amount 
     * @param flags 
     */
    void MapVirtualMemory4K(VirtualPages *pagemap, uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags);
    inline void MapVirtualMemory4K(VirtualPages *pagemap, uint64_t phys, uint64_t virt, size_t amount) {
        MapVirtualMemory4K(pagemap, phys, virt, amount, PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER);
    }
    /**
     * @brief 
     * 
     * @param amount 
     * @param pageSpace 
     * @return uintptr_t 
     */
    uintptr_t Allocate4KPages(VirtualPages *pagemap, size_t amount);
    /**
     * @brief 
     * 
     * @param virt 
     * @param amount 
     * @param addressSpace 
     */
    void Free4KPages(VirtualPages *pagemap, uint64_t virt, size_t amount);

    /**
     * @brief 
     * 
     * @param phys 
     * @param virt 
     * @param amount 
     * @param flags 
     */
    void KernelMapVirtualMemory2M(uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags);
#define KernelMapVirtualMemory2M(phys, virt, amount) KernelMapVirtualMemory2M(phys, virt, amount, PAGE_PRESENT | PAGE_WRITABLE);
    /**
     * @brief 
     * 
     * @param phys 
     * @param virt 
     * @param amount 
     */
    void KernelMapVirtualMemory4K(uint64_t phys, uint64_t virt, size_t amount);
    /**
     * @brief 
     * 
     * @param phys 
     * @param virt 
     * @param amount 
     * @param flags 
     */
    void KernelMapVirtualMemory4K(uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags);
    /**
     * @brief 
     * 
     * @param amount 
     * @return uintptr_t 
     */
    uintptr_t KernelAllocate4KPages(size_t amount);
    /**
     * @brief 
     * 
     * @param addr 
     * @param amount 
     */
    void KernelFree4KPages(void* addr, uint64_t amount);

    /**
     * @brief 
     * 
     * @param pml4 
     * @param addr 
     * @return uintptr_t 
     */
    uintptr_t ConvertVirtToPhys(VirtualPages *pagemap, uintptr_t addr);

    inline void SetPageAddress(uint64_t *page, uint64_t addr) { *page = (*page & ~PAGE_FRAME) | (addr & PAGE_FRAME); }

    inline void SetPageFlags(uint64_t *page, uint64_t flags) { *page |= flags; }

    inline void SetPageFrame(uint64_t *page, uint64_t addr, uint64_t flags) { *page = (*page & ~PAGE_FRAME) | (addr & PAGE_FRAME) | flags; }

    inline uintptr_t GetIOMapping(uint64_t addr) {
        if (addr > 0xFFFFFFFF) {
            System::Panic("Access IO address (%x) greater than 4GB", addr);
            return 0xFFFFFFFF;
        }
        return addr + IO_VIRTUAL_BASE;
    }

    extern VirtualPages g_KernelPagemap;
} // namespace Memory

extern "C" {
Paging::pml4_t* asmw_get_pagemap();
inline void asmi_load_paging(uintptr_t addr) { asm("mov %%rax, %%cr3" ::"a"((uint64_t)addr)); }
inline void asmi_invlpg(uintptr_t addr) { asm volatile("invlpg (%0)"::"r"(addr)); }
}