#pragma once

#include <Macros>
#include <Memory/Memory.h>
#include <Process/Process.h>
#include <Utils/Range.h>

#define ARCH_PAGE_SIZE              (4096)
#define PAGES_PER_TABLE             512
#define TABLES_PER_DIR              512
#define DIRS_PER_PDPT               512
#define PDPTS_PER_PML4              512
#define PML4_GET_INDEX(addr)        (((addr) >> 39) & 0x1FF)
#define PDPT_GET_INDEX(addr)        (((addr) >> 30) & 0x1FF)
#define PDIR_GET_INDEX(addr)        (((addr) >> 21) & 0x1FF)
#define PAGE_GET_INDEX(addr)        (((addr) >> 12) & 0x1FF)
#define PAGE_FLAG_PRESENT           (1 << 0)
#define PAGE_FLAG_WRITABLE          (1 << 1)
#define PAGE_FLAG_USER              (1 << 2)
#define PAGE_FLAG_WRITETHROUGH      (1 << 3)
#define PAGE_FLAG_NOCACHE           (1 << 4)
#define IS_PAGE_ALIGNED(addr)       (addr % ARCH_PAGE_SIZE == 0)

typedef uint64_t page_flags_t;

namespace Memory
{
    typedef struct PML4Entry
    {
        bool present : 1;       // Must be 1
        bool writable : 1;      // Page is readonly if set to 0, also called Read/Write bit
        bool usr : 1;           // Everyone could access this page if it's not 0, or only supervisor allowed.
        bool writeThrough : 1; // Write-Through cache is enabled when this bit is set
        bool cache : 1;         // Disable cache if it's set to 1
        bool access : 1;        // Is this page entry has been used.
        int ign_6_11 : 6;       // Ignored
        uint64_t addr : 36;     // Physical Address
        int ign_48_62 : 15;     // Ignored
        bool disableExec: 1;
    } __attribute__((packed)) pml4_entry_t;

    typedef struct PdptEntry
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
    } __attribute__((packed)) pdpt_entry_t;

    typedef struct PdirEntry
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
    } __attribute__((packed)) pd_entry_t;

    typedef struct PageEntry
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
    } __attribute__((packed)) page_t;

    using PML4Map = PML4Entry[PDPTS_PER_PML4]; /* 512GiB -> 256TiB */
    using Pdpt = PdptEntry[DIRS_PER_PDPT]; /* 1GiB -> 512GiB */
    using PageDirectory = PdirEntry[TABLES_PER_DIR]; /* 2MiB -> 1GiB */
    using PageTable = PageEntry[PAGES_PER_TABLE]; /* 4KiB -> 2MiB */

    struct VirtualPages
    {
        PML4Map mapEntry;
        PdptEntry *pdpts[PDPTS_PER_PML4];
        PdirEntry **pageDirs[DIRS_PER_PDPT];
        PageEntry ***pageTables[TABLES_PER_DIR];
    };

    /**
     * @brief Initialize kernel page map and virtual memory management
     */
    void LoadVirtualMemory(void);
    /**
     * @brief 
     * 
     * @param map 
     * @param addr 
     * @return true If the page is present (in the memory)
     * @return false If the page does not exist or be swapped into disks.
     */
    bool IsPagePresent(
        PML4Map *map, 
        uint64_t addr
    );
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
    uintptr_t GetIOMapping(uintptr_t address);

    /**
     * @brief 
     * 
     * @param map 
     * @param phys 
     * @param virt 
     * @param amount 
     * @param flags 
     */
    void MapVirtualAddress(PML4Map *map, uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags);
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
     * @param flags 
     */
    void KernelMapVirtualPages(uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags);
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
    uintptr_t ConvertVirtToPhys(PML4Map *pml4, uintptr_t addr);

    void LoadVirtualMemoryPages(PML4Map *map);
    PML4Map *GetVirtualPages();
    PML4Map *GetKernelPages();
    page_t *GetVirtualPageDesc(PML4Map *map, uintptr_t addr);
} // namespace Memory

extern Memory::PML4Map* asmw_get_pagemap(void);
static inline void asmi_load_paging(uintptr_t addr) { asm("mov %%rax, %%cr3" ::"a"((uint64_t)addr)); }
static inline void asmi_invlpg(uintptr_t addr) { asm volatile("invlpg (%0)"::"r"(addr)); }