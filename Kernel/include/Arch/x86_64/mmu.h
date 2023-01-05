#pragma once

#include <macros>
#include <mm/mem.h>
#include <proc/proc.h>
#include <utils/range.h>

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
#define IS_PAGE_ALIGNED(addr)       (addr % ARCH_PAGE_SIZE == 0)

namespace Memory::ManagementUnit
{
    /*
    struct Pml4Entry
    {
        bool present : 1;       // Must be 1
        bool writable : 1;      // Page is readonly if set to 0, also called Read/Write bit
        bool usr : 1;           // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough : 1; // Write-Through cache is enabled when this bit is set
        bool cache : 1;         // Disable cache if it's set to 1
        bool access : 1;        // Is this page entry has been used.
        int ign_6_11 : 6;       // Ignored
        uint64_t address : 36;     // Physical Address
        int ign_48_62 : 15;     // Ignored
        bool disableExecution: 1;
    } __attribute__((packed));

    struct PdptEntry
    {
        bool present : 1;       // Must be 1
        bool writable : 1;      // Page is readonly if set to 0, also called Read/Write bit
        bool usr : 1;           // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough : 1;  // Write-Through cache is enabled when this bit is set
        bool cache : 1;         // Disable cache if it's set to 1
        bool access : 1;        // Is this page entry has been used.
        int ign_6 : 1;
        int size : 1;
        int ign_8_11 : 4;
        uint64_t addr : 36;
        int ign_48_62 : 15;
        bool disableExec : 1;
    } __attribute__((packed));

    struct PdirEntry
    {
        bool present : 1;       // Must be 1
        bool writable : 1;      // Page is readonly if set to 0, also called Read/Write bit
        bool usr : 1;           // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough : 1;  // Write-Through cache is enabled when this bit is set
        bool cache : 1;         // Disable cache if it's set to 1
        bool access : 1;        // Is this page entry has been used.
        int ign_6 : 1;
        int size : 1;
        int ign_8_11 : 4;
        uint64_t addr : 36;
        int ign_48_62 : 15;
        bool disableExec : 1;
    } __attribute__((packed));

    struct PageEntry
    {
        bool present : 1;      // Must be 1
        bool writable : 1;     // Page is readonly if set to 0, also called Read/Write bit
        bool usr : 1;          // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough : 1; // Write-Through cache is enabled when this bit is set
        bool cache : 1;        // Disable cache if it's set to 1
        bool access : 1;       // Is this page entry has been used.
        int dirty : 1;
        int memType : 1;
        int global : 1;
        int ign_9_11 : 3;
        uint64_t addr : 36;
        int ign_48_57 : 10;
        uint8_t key : 5;
        bool disableExec : 1;
    } __attribute__((packed));
    */

    typedef uint64_t Pml4Entry;
    typedef uint64_t PdptEntry;
    typedef uint64_t PdirEntry;
    typedef uint64_t PageEntry;

    using Pml4EntrySet = Pml4Entry[PDPTS_PER_PML4]; /* 512GiB -> 256TiB */
    using PdptEntrySet = PdptEntry[DIRS_PER_PDPT]; /* 1GiB -> 512GiB */
    using PdirEntrySet = PdirEntry[TABLES_PER_DIR]; /* 2MiB -> 1GiB */
    using PageEntrySet = PageEntry[PAGES_PER_TABLE]; /* 4KiB -> 2MiB */

    struct VirtualPages
    {
        pml4_t pml4;
        PdptEntry *pdpts[PDPTS_PER_PML4];
        PdirEntry **pageDirs[DIRS_PER_PDPT];
        PageEntry ***pageTables[TABLES_PER_DIR];
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
    bool IsPagePresent(Pml4Set *map, uint64_t addr);
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
     * @param address 
     * @return uintptr_t 
     */
    uintptr_t GetIOMapping(address_t addr);

    /**
     * @brief 
     * 
     * @param map 
     * @param phys 
     * @param virt 
     * @param amount 
     * @param flags 
     */
    void MapVirtualAddress(Pml4Set *map, uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags);
    /**
     * @brief 
     * 
     * @param amount 
     * @param pageSpace 
     * @return uintptr_t 
     */
    uintptr_t Allocate4KPages(size_t amount, VirtualPages *pageSpace);
    /**
     * @brief 
     * 
     * @param virt 
     * @param amount 
     * @param addressSpace 
     */
    void Free4KPages(
        uint64_t virt,
        size_t amount,
        VirtualPages *pageSpace
    );

    /**
     * @brief 
     * 
     * @param phys 
     * @param virt 
     * @param amount 
     */
    void KernelMapVirtualAddress(uint64_t phys, uint64_t virt, size_t amount);
    /**
     * @brief 
     * 
     * @param phys 
     * @param virt 
     * @param amount 
     * @param flags 
     */
    void KernelMapVirtualAddress(uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags);
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
    uintptr_t ConvertVirtToPhys(Pml4Set *pml4, uintptr_t addr);

    inline void SetPageAddress(uint64_t *page, uint64_t addr) { *page = (*page & ~PAGE_FRAME) | (addr & PAGE_FRAME); }

    inline void SetPageFlags(uint64_t *page, uint64_t flags) { *page |= flags; }

    inline void SetPageFrame(uint64_t *page, uint64_t addr, uint64_t flags) { *page = (*page & ~PAGE_FRAME) | (addr & PAGE_FRAME) | flags; }

    void LoadVirtualMemoryPages(Pml4Set *map);
    Pml4Set *GetVirtualPages();
    Pml4Set *GetKernelPages();
    page_t *GetVirtualPageDesc(Pml4Set *map, uintptr_t addr);
} // namespace Memory

extern "C" {
Memory::Pml4Set* asmw_get_pagemap();
inline void asmi_load_paging(uintptr_t addr) { asm("mov %%rax, %%cr3" ::"a"((uint64_t)addr)); }
inline void asmi_invlpg(uintptr_t addr) { asm volatile("invlpg (%0)"::"r"(addr)); }
}