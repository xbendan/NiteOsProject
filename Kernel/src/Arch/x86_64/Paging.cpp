#include <Arch/x86_64/IDT.h>
#include <Arch/x86_64/Paging.h>
#include <Memory/Memory.h>
#include <Memory/KMalloc.h>
#include <Memory/Page.h>
#include <Process/Scheduler.h>
#include <Address>
#include <Kernel>

namespace Memory
{
    PML4Map         kernelPages __attribute__((aligned(ARCH_PAGE_SIZE)));
    Pdpt            kernelPdpts __attribute__((aligned(ARCH_PAGE_SIZE)));
    PageDirectory   kernelPageDirs[DIRS_PER_PDPT] __attribute__((aligned(ARCH_PAGE_SIZE)));
    PageTable       kernelPageTables[TABLES_PER_DIR] __attribute__((aligned(ARCH_PAGE_SIZE)));

    PageTable      *kernelPageTablePointers[DIRS_PER_PDPT][TABLES_PER_DIR];

    PML4Map *currentPages;

    uint64_t kernelPageMarks[8192];

    void Interrupts_PageFaultHandler(void *, struct RegisterContext *regs)
    {
        //WriteLine("[ERR] Page Fault");
        asm("hlt");
    }

    void LoadVirtualMemory()
    {
        PML4Entry *pml4Entry = &kernelPages[PDPTS_PER_PML4 - 1];

        pml4Entry->present = 1;
        pml4Entry->writable = 1;
        pml4Entry->usr = 0;

        pml4Entry->addr = (((uint64_t) &kernelPdpts) - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE;

        for (size_t pdptIndex = 0; pdptIndex < DIRS_PER_PDPT; pdptIndex++)
        {
            PdptEntry *pdptEntry = &kernelPdpts[pdptIndex];

            pdptEntry->present = 1;
            pdptEntry->writable = 1;
            pdptEntry->usr = 0;

            pdptEntry->addr = (((uint64_t) &kernelPageDirs[pdptIndex]) - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE;
        }

        for (size_t pdirIndex = 0; pdirIndex < TABLES_PER_DIR; pdirIndex++)
        {
            PdirEntry *pdirEntry = &kernelPageDirs[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)][pdirIndex];

            pdirEntry->present = 1;
            pdirEntry->writable = 1;
            pdirEntry->usr = 0;

            PageTable *pageTable = &kernelPageTables[pdirIndex];

            pdirEntry->addr = (((uint64_t) pageTable) - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE;
            kernelPageTablePointers[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)][pdirIndex] = pageTable;
        }

        Proc::Process *kernelProcess = Proc::GetKernelProcess();
        kernelProcess->m_Pagemap = &kernelPages;
        kernelProcess->m_VirtuaPagesBitmap = (uint64_t **) &kernelPageMarks;
        for (size_t idx = 0; idx < 16; idx++)
            kernelProcess->m_VirtuaPagesBitmap[idx] = &kernelPageMarks[idx * 512];

        uint64_t kAddrStart = ALIGN_DOWN((uintptr_t) &KERNEL_START_ADDR, ARCH_PAGE_SIZE);
        uint64_t kPageAmount = (ALIGN_UP((uintptr_t) &KERNEL_END_ADDR, ARCH_PAGE_SIZE) - kAddrStart) / ARCH_PAGE_SIZE;
        
        MapVirtualAddress(
            &kernelPages,
            kAddrStart - KERNEL_VIRTUAL_BASE,
            kAddrStart,
            kPageAmount,
            PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE
        );

        /*
        MarkVirtualPagesUsed(
            kernelProcess,
            KERNEL_VIRTUAL_BASE,
            (kAddrStart - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE
        );
        MarkVirtualPagesUsed(
            kernelProcess,
            kAddrStart,
            kPageAmount
        );
        */

        //RegisterInterruptHandler(14, &Interrupts_PageFaultHandler);

        LoadVirtualMemoryPages(GetKernelPages());
    }

    bool IsPagePresent(
        PML4Map *map,
        uint64_t addr)
    {
        size_t pml4Index = PML4_GET_INDEX(addr);

        if(pml4Index == PDPTS_PER_PML4 - 1)
        {
            // Kernel address space
            PageEntry *page = GetVirtualPageDesc(map, addr);
            return page != NULL && page->present;
        }
        else
        {
            size_t pdptIndex = PDPT_GET_INDEX(addr), pdirIndex = PDIR_GET_INDEX(addr), pageIndex = PAGE_GET_INDEX(addr);
        }
    }

    VirtualPages *CreatePagemap()
    {

    }

    void DestroyPagemap()
    {

    }

    uintptr_t GetIOMapping(uintptr_t address)
    {
        if(address > 0xFFFFFFFF)
        {
            // WriteLine("[WARN] Access IO address greater than 0xFFFFFFFF.");
            // WriteLong(address);
            return 0xFFFFFFFF;
        }
        return address + KERNEL_IO_VIRTUAL_BASE;
    }

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

    void KernelMapVirtualPages(uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags)
    {
        size_t pdptIndex, pdirIndex, pageIndex;

        while(amount)
        {
            /* Kernel Address Space */
            PageEntry *page = GetVirtualPageDesc(map, virt);
            if (page == NULL)
            {
                PdirEntry *pdirEntry = &kernelPageDirs[pdptIndex][pdirIndex];
                PageTable *pageTable;
                if (!pdirEntry->present)
                {
                    pdirEntry->present = 1;
                    pdirEntry->writable = flags & PAGE_FLAG_WRITABLE;
                    pdirEntry->usr = flags & PAGE_FLAG_USER;

                    pageTable = (PageTable *) kallocpg(1);
                    uint64_t pageTablePhys = ConvertVirtToPhys(map, (uint64_t)(pageTable));

                    pdirEntry->addr = pageTablePhys / ARCH_PAGE_SIZE;
                    kernelPageTablePointers[pdptIndex][pdirIndex] = pageTable;
                }
                else
                {
                    pageTable = kernelPageTablePointers[pdptIndex][pdirIndex];
                }

                page = &pageTable[pageIndex];
            }

            page->present = 1;
            page->writable = flags & PAGE_FLAG_WRITABLE;
            page->usr = flags & PAGE_FLAG_USER;

            page->addr = phys / ARCH_PAGE_SIZE;

            phys += ARCH_PAGE_SIZE;
            virt += ARCH_PAGE_SIZE;
            amount--;
        }
    }

    /**
     * @brief 
     * 
     * @param amount 
     * @return uintptr_t 
     */
    uintptr_t KernelAllocate4KPages(size_t amount)
    {
        uint64_t counter;
        uintptr_t address;

        for (int i = 0; i)
    }

    /**
     * @brief 
     * 
     * @param addr 
     * @param amount 
     */
    void KernelFree4KPages(void* addr, uint64_t amount);

    void MapVirtualAddress(
        PML4Map *map,
        uint64_t phys,
        uint64_t virt,
        size_t amount,
        page_flags_t flags)
    {
        bool writable = flags & PAGE_FLAG_WRITABLE;
        bool usr = flags & PAGE_FLAG_USER;
        Proc::Process *kernelProc = Proc::GetKernelProcess();
        size_t pml4Index, pdptIndex, pdirIndex, pageIndex;

        while (amount)
        {
            pml4Index = PML4_GET_INDEX(virt);
            pdptIndex = PDPT_GET_INDEX(virt);
            pdirIndex = PDIR_GET_INDEX(virt);
            pageIndex = PAGE_GET_INDEX(virt);

            if(pml4Index == PDPTS_PER_PML4 - 1)
            {
                /* Kernel Address Space */
                PageEntry *page = GetVirtualPageDesc(map, virt);
                if(page == NULL)
                {
                    PdirEntry *pdirEntry = &kernelPageDirs[pdptIndex][pdirIndex];
                    PageTable *pageTable;
                    if(!pdirEntry->present)
                    {
                        pdirEntry->present = 1;
                        pdirEntry->writable = flags & PAGE_FLAG_WRITABLE;
                        pdirEntry->usr = flags & PAGE_FLAG_USER;
                        
                        pageTable = (PageTable *) kallocpg(1);
                        uint64_t pageTablePhys = ConvertVirtToPhys(map, (uint64_t)(pageTable));
                        
                        pdirEntry->addr = pageTablePhys / ARCH_PAGE_SIZE;
                        kernelPageTablePointers[pdptIndex][pdirIndex] = pageTable;
                    }
                    else
                    {
                        pageTable = kernelPageTablePointers[pdptIndex][pdirIndex];
                    }
                    
                    page = &pageTable[pageIndex];
                }

                page->present = 1;
                page->writable = flags & PAGE_FLAG_WRITABLE;

                page->addr = phys / ARCH_PAGE_SIZE;
            }
            else
            {
                asm("hlt");
            }

            phys += ARCH_PAGE_SIZE;
            virt += ARCH_PAGE_SIZE;
            amount--;
        }
    }

    uintptr_t AllocateVirtualPages(
        Proc::Process *process,
        size_t amount)
    {
        if(process == NULL)
            process = Proc::GetCurrentProcess();
            

        uintptr_t virt = 0;
        bool is_kernel_space = (process == Proc::GetKernelProcess());
        size_t amount_left;
        size_t space_limit = (is_kernel_space ? 16 : 16 * 256);
        uint64_t** marks = process->m_VirtuaPagesBitmap;
        /*
        * Each process can take upto 512 GiB virtual address
        * which is 512GiB / 128MiB = 8 * 512 = 4096
        */
        for (size_t idx = 0; idx < space_limit; idx++)
        {
            /* 
            * Determine whether the pointer point to the page
            * contains page marks is null or not.
            * 
            * If it's a null pointer try to allocate a new page.
            */
            if (marks[idx] == NULL)
            {
                /* Try to allocate and map 1 page (4096 KiB) for saving marks. */
                uint64_t virt_mark = kallocpg(1);
                if(virt_mark)
                    marks[idx] = (uint64_t *) virt_mark;
                else
                {
                    /*
                    * If the allocated address is 0 (failed)
                    * Throwing errors about out of address
                    */
                    // panic.
                }
            }

            /* Scan 128 MiB virtual address each time */
            for (size_t sub_index = 0; sub_index < 512; sub_index++)
            {
                /* Skip to next mark if all bits set */
                if (marks[idx][sub_index] == 0xffffffffffffffff)
                    continue;

                /* Iterate all bits for a free bit */
                for (uint8_t bit_index = 0; bit_index < 64; bit_index++)
                {
                    /*
                    * Check whether the bit indicate the page availablitiy
                    * is free or not. 
                    * 
                    * If yes, start or continue counting on the page amount
                    * Otherwise, clear virtual address recorded and prepare
                    * to restart counting.
                    */
                    if (!(marks[idx][sub_index] & (1 << bit_index)))
                    {
                        /*
                        * If the virtual address is empty, set a new address and
                        * start to count the pages left.
                        */
                        if (!virt)
                        {
                            virt = ((idx * 32768) + (sub_index * 64) + bit_index) * ARCH_PAGE_SIZE;
                            amount_left = amount;
                        }

                        /*
                        * Decrease the amount left and check whether the space taken
                        * is enough for requirement.
                        * 
                        * If yes, mark the virtual memory pages used and return the 
                        * address. (Add kernel virtual base if we are allocating in
                        * kernel address space)
                        * If no, continue to the next loop.
                        */
                        if (!--amount_left)
                        {
                            if(is_kernel_space)
                                virt += KERNEL_VIRTUAL_BASE;
                            MarkVirtualPagesUsed(process, virt, amount);
                            return virt;
                        }
                    }
                    else
                        virt = 0;
                }
            }
        }

        return 0;
    }

    void FreeVirtualPages(
        Proc::Process *process,
        uint64_t virt,
        size_t amount)
    {
        MarkVirtualPagesFree(process, virt, amount);
    }

    void MarkVirtualPagesUsed(
        Proc::Process *process,
        uint64_t virt, 
        size_t amount)
    {

        if(process == Proc::GetKernelProcess())
            virt -= KERNEL_VIRTUAL_BASE;

        uint16_t pageIndex, ptr_index;
        uint8_t bit_index;

        uint64_t **marks = process->m_VirtuaPagesBitmap;
        
        virt /= ARCH_PAGE_SIZE;
        while (amount)
        {
            ptr_index = virt / 32768;
            pageIndex = virt / 64;
            bit_index = virt % 64;

            process->m_VirtuaPagesBitmap[ptr_index][pageIndex] |= (1 << bit_index);
            virt++;
            amount--;

            if(amount <= 0)
            {
                return;
            }
        }
    }

    void MarkVirtualPagesFree(
        Proc::Process *process,
        uint64_t virt,
        size_t amount)
    {
        if(process == Proc::GetKernelProcess())
            virt -= KERNEL_ADDR_SPACE;

        uint16_t pageIndex, ptr_index;
        uint8_t bit_index;
            
        virt /= ARCH_PAGE_SIZE;
        while (amount)
        {
            ptr_index = virt / 32768;
            pageIndex = virt / 64;
            bit_index = virt % 64;

            process->m_VirtuaPagesBitmap[ptr_index][pageIndex] |= (1 << bit_index);
            virt++;
            amount--;

            if(amount <= 0)
                return;
        }
    }

    uintptr_t ConvertVirtToPhys(
        PML4Map *map, 
        uintptr_t addr)
    {
        uint64_t address = 0;
        size_t pml4Index = PML4_GET_INDEX(addr), pdptIndex = PDPT_GET_INDEX(addr), pdirIndex = PDIR_GET_INDEX(addr), pageIndex = PAGE_GET_INDEX(addr);

        if(pml4Index == PDPTS_PER_PML4 - 1)
        {
            PageTable *pageTable = kernelPageTablePointers[pdptIndex][pdirIndex];

            if(pageTable != 0x0 && pageTable != NULL)
            {
                return (pageTable[pageIndex].addr * ARCH_PAGE_SIZE) + (addr & 0xFFF);
            }
        }
        else
        {

        }

        return addr;
    }

    PageEntry *GetVirtualPageDesc(PML4Map *map, uintptr_t addr)
    {
        size_t pml4Index, pdptIndex, pdirIndex, pageIndex;

        pml4Index = PML4_GET_INDEX(addr);
        pdptIndex = PDPT_GET_INDEX(addr);
        pdirIndex = PDIR_GET_INDEX(addr);
        pageIndex = PAGE_GET_INDEX(addr);

        if(pml4Index == (PDPTS_PER_PML4 - 1))
        {
            // Kernel Address Space
            PageTable *pageTableEntry = kernelPageTablePointers[pdptIndex][pdirIndex]; //kernelPageDirs[pdptIndex].entries[pdirIndex];
            if (pageTableEntry != 0x0 && pageTableEntry != NULL)
                return &pageTableEntry[pageIndex];
            else
                return NULL;
        }
        else
        {
            // Process Address Space
            return NULL;
        }
    }

    void LoadVirtualMemoryPages(PML4Map *map)
    {
        asmi_load_paging(
            ConvertVirtToPhys(
                GetKernelPages(), 
                (uintptr_t) map
            )
        );
    }

    PML4Map* GetVirtualPages() { return asmw_get_pagemap(); }
    PML4Map* GetKernelPages() { return &kernelPages; }
} // namespace Memory