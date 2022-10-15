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

typedef struct Pml4Entry
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

typedef struct page_map
{
    pml4_t mapEntry;
    pdpt_t *pdpts[PDPTS_PER_PML4];
    page_dir_t **pageDirs[DIRS_PER_PDPT];
    page_t ***pageTables[TABLES_PER_DIR];
} page_map_t;

using Pml4Map = Pml4Entry[PDPTS_PER_PML4]; /* 512GiB -> 256TiB */
using Pdpt = PdptEntry[DIRS_PER_PDPT]; /* 1GiB -> 512GiB */
using Pdir = PdirEntry[TABLES_PER_DIR]; /* 2MiB -> 1GiB */
using Ptable = PageEntry[PAGES_PER_TABLE]; /* 4KiB -> 2MiB */

typedef uint64_t page_flags_t;

extern pml4_t* asmw_get_pagemap(void);
static inline void asmi_load_paging(uintptr_t addr) { asm("mov %%rax, %%cr3" ::"a"((uint64_t)addr)); }
static inline void asmi_invlpg(uintptr_t addr) { asm volatile("invlpg (%0)"::"r"(addr)); }
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
    pml4_t *map, 
    uint64_t addr
);
page_map_t *CreatePagemap();
void DestoryPagemap();
uintptr_t GetIOMapping(uintptr_t address);
void MapVirtualAddress(
    pml4_t *map,
    uint64_t phys,
    uint64_t virt,
    size_t amount,
    page_flags_t flags
);
uintptr_t AllocateVirtualPages(
    struct Process *process,
    size_t amount
);
void FreeVirtualPages(
    struct Process *process,
    uint64_t virt,
    size_t amount
);
void MarkVirtualPagesUsed(
    struct Process *process,
    uint64_t virt, 
    size_t amount
);
void MarkVirtualPagesFree(
    struct Process *process,
    uint64_t virt,
    size_t amount
);
uintptr_t ConvertVirtToPhys(
    pml4_t *pml4, 
    uintptr_t addr
);
void LoadVirtualMemoryPages(pml4_t *map);
pml4_t *GetVirtualPages();
pml4_t *GetKernelPages();
page_t *GetVirtualPageDesc(pml4_t *map, uintptr_t addr);
