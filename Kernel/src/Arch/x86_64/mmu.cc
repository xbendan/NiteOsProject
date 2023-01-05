#include <Arch/x86_64/idt.h>
#include <Arch/x86_64/mmu.h>
#include <mm/mem.h>
#include <mm/kmalloc.h>
#include <mm/page.h>
#include <proc/sched.h>
#include <utils/spinlock.h>
#include <address.h>
#include <kern.h>

namespace Memory::ManagementUnit
{
    Pml4EntrySet    kernelPages __attribute__((aligned(ARCH_PAGE_SIZE)));
    PdptEntrySet    kernelPdpts __attribute__((aligned(ARCH_PAGE_SIZE)));
    PdirEntrySet    kernelPageDirs __attribute__((aligned(ARCH_PAGE_SIZE)));
    PdirEntrySet    ioMappings[4] __attribute__((aligned(ARCH_PAGE_SIZE)));
    PageEntrySet    kernelPageTables[TABLES_PER_DIR] __attribute__((aligned(ARCH_PAGE_SIZE)));

    uint64_t    kernelPageMarks[8192];

    Pml4EntrySet* GetVirtualPages() { return asmw_get_pagemap(); }
    Pml4EntrySet* GetKernelPages() { return &kernelPages; }


    /*
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
    }*/

    void InitializeVirtualMemory() {
        memset(&kernelPages, 0, sizeof(Pml4EntrySet));
        memset(&kernelPdpts, 0, sizeof(PdptEntrySet));
        memset(&kernelPageDirs, 0, sizeof(PdirEntrySet));

        SetPageFrame(&kernelPages[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)], &kernelPdpts - KERNEL_VIRTUAL_BASE, 0x3);
        kernelPages[0] = kernelPages[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)];

        SetPageFrame(&kernelPdpts[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)], &kernelPageDirs - KERNEL_VIRTUAL_BASE, 0x3);
        SetPageFrame(&kernelPdpts[DIRS_PER_PDPT - 1], &kernelHeapPageDirs - KERNEL_VIRTUAL_BASE, 0x3);
        for (int j = 0; j < TABLES_PER_DIR; j++) {
            kernelPageDirs[j] = (PAGE_SIZE_2M * j) | 0x83;
        }
        SetPageFrame(&kernelPdpts[DIRS_PER_PDPT - 1], &kernelHeapPageTables - KERNEL_VIRTUAL_BASE, 0x3);

        for (int i = 0; i < 4; i++) {
            SetPageFrame(kernelPdpts[PDPT_GET_INDEX(IO_VIRTUAL_BASE) + i], ((address_t)&(ioMappings[i])) - KERNEL_VIRTUAL_BASE, 0x3);
            for (int j = 0; j < TABLES_PER_DIR; j++) {
                SetPageFrame(&ioMappings[i][j], (PAGE_SIZE_1G * i + PAGE_SIZE_2M * j), 0x83);
            }
        }
        kernelPdpts[0] = kernelPdpts[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)];

        KernelMapVirtualMemory4K(
            0x0, 
            KERNEL_VIRTUAL_BASE, 
            (&KERNEL_END_ADDR - &KERNEL_START_ADDR) / ARCH_PAGE_SIZE);

        asm("mov %%rax, %%cr3" ::"a"((address_t)&kernelPages - KERNEL_VIRTUAL_BASE));
    }

    /*
    void LoadVirtualMemory()
    {
        Pml4Entry *pml4Entry = &(kernelPages[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)]);

        pml4Entry->present = 1;
        pml4Entry->writable = 1;
        pml4Entry->usr = 0;

        pml4Entry->addr = (((uint64_t) &kernelPdpts) - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE;

        for (size_t pdptIndex = 0; pdptIndex < DIRS_PER_PDPT; pdptIndex++)
        {
            PdptEntry *pdptEntry = &(kernelPdpts[pdptIndex]);

            pdptEntry->present = 1;
            pdptEntry->writable = 1;
            pdptEntry->usr = 0;

            pdptEntry->addr = (((uint64_t) &kernelPageDirs[pdptIndex]) - KERNEL_VIRTUAL_BASE) / ARCH_PAGE_SIZE;
        }

        for (size_t pdirIndex = 0; pdirIndex < TABLES_PER_DIR; pdirIndex++)
        {
            PdirEntry *pdirEntry = &(kernelPageDirs[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)][pdirIndex]);

            pdirEntry->present = 1;
            pdirEntry->writable = 1;
            pdirEntry->usr = 0;

            VmPageTable *pageTable = &kernelPageTables[pdirIndex];

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
        
        KernelMapVirtualMemory4K(
            kAddrStart - KERNEL_VIRTUAL_BASE,
            kAddrStart,
            kPageAmount,
            PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE
        );

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

        //RegisterInterruptHandler(14, &Interrupts_PageFaultHandler);
        LoadVirtualMemoryPages(GetKernelPages());
    }
    */

    void HandlePageFault(void *ptr, RegisterContext *regs) {

    }

    bool IsPagePresent(
        VmPages *map,
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
        return NULL;
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
    void MapVirtualMemory4K(VmPages *map, uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags)
    {

    }

    /**
     * @brief 
     * 
     * @param amount 
     * @param pageSpace 
     * @return uintptr_t 
     */
    uintptr_t Allocate4KPages(size_t amount, VirtualPages *pageSpace)
    {

    }

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
    
    void KernelMapVirtualMemory4K(uint64_t phys, uint64_t virt, size_t amount) {
        KernelMapVirtualMemory4K(phys, virt, amount, PAGE_PRESENT | PAGE_WRITABLE);
    }

    void KernelMapVirtualMemory4K(uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags)
    {
        size_t pdptIndex, pdirIndex, pageIndex;

        while (amount) {
            pdirIndex = PDIR_GET_INDEX(virt);
            pageIndex = PAGE_GET_INDEX(virt);
        }

        while(amount)
        {
            pdptIndex = PDPT_GET_INDEX(virt);
            pdirIndex = PDIR_GET_INDEX(virt);
            pageIndex = PAGE_GET_INDEX(virt);

            /* Kernel Address Space */
            PageEntry *page = GetVirtualPageDesc(&kernelPages, virt);
            if (page == NULL)
            {
                PdirEntry *pdirEntry = &kernelPageDirs[pdptIndex][pdirIndex];
                VmPageTable *pageTable;
                if (!pdirEntry->present)
                {
                    pdirEntry->present = 1;
                    pdirEntry->writable = flags & PAGE_FLAG_WRITABLE;
                    pdirEntry->usr = flags & PAGE_FLAG_USER;

                    pageTable = (VmPageTable *) kallocpg(1);
                    uint64_t pageTablePhys = ConvertVirtToPhys(&kernelPages, (uint64_t)(pageTable));

                    pdirEntry->addr = pageTablePhys / ARCH_PAGE_SIZE;
                    kernelPageTablePointers[pdptIndex][pdirIndex] = pageTable;
                }
                else
                {
                    pageTable = kernelPageTablePointers[pdptIndex][pdirIndex];
                }

                page = &((*pageTable)[pageIndex]);
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
        uint64_t offset, pageDirOffset;
        uint64_t counter;
        uintptr_t address;

        for (int i = 0; i < TABLES_PER_DIR; i++) {
            if(kernelPageDirs[i][0].addr && !kernelPageDirs[i][0].size)  {
                for (int j = 0; j < PAGES_PER_TABLE; j++) {
                    if(kernelPageTablePointers[i][j] & 0x1) {
                        pageDirOffset = i;
                        offset = j + 1;
                        counter = 0;
                        continue;
                    }

                    counter++;

                    if(counter >= amount) {
                        address = ((PDPT_SIZE * 511) + (PAGE_SIZE_1G * 511) + (pageDirOffset * PAGE_SIZE_2M) + 
                                (offset * PAGE_SIZE_4K)) | 0xFFFF000000000000;
                        while (counter--) {
                            if (offset >= 512) {
                                pageDirOffset++;
                                offset = 0;
                            }
                            struct PageEntry *page = &kernelPageTables[pageDirOffset][offset];
                            page->present = 1;
                            page->writable = 1;
                            
                            offset++;
                        }
                        return address;
                    }
                }
            } else {
                pageDirOffset = i + 1;
                offset = 0;
                counter = 0;
            }
        }

        pageDirOffset = 0;
        offset = 0;
        counter = 0;

        for (int i = 0; i < TABLES_PER_DIR; i++) {
            if (!())
        }
    }

    /**
     * @brief 
     * 
     * @param addr 
     * @param amount 
     */
    void KernelFree4KPages(void* addr, uint64_t amount);

    void MapVirtualMemory4K(
        VmPages *map,
        uint64_t phys,
        uint64_t virt,
        size_t amount,
        page_flags_t flags)
    {
        bool writable = flags & PAGE_FLAG_WRITABLE;
        bool usr = flags & PAGE_FLAG_USER;
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
                    VmPageTable *pageTable;
                    if(!pdirEntry->present)
                    {
                        pdirEntry->present = 1;
                        pdirEntry->writable = writable;
                        pdirEntry->usr = usr;
                        
                        pageTable = (VmPageTable *) kallocpg(1);
                        uint64_t pageTablePhys = ConvertVirtToPhys(map, (uint64_t)(pageTable));
                        
                        pdirEntry->addr = pageTablePhys / ARCH_PAGE_SIZE;
                        kernelPageTablePointers[pdptIndex][pdirIndex] = pageTable;
                    }
                    else
                    {
                        pageTable = kernelPageTablePointers[pdptIndex][pdirIndex];
                    }
                    
                    page = &(*pageTable)[pageIndex];
                }

                page->present = 1;
                page->writable = writable;

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
        //MarkVirtualPagesFree(process, virt, amount);
    }

    uintptr_t ConvertVirtToPhys(
        VmPages *map, 
        uintptr_t addr)
    {
        uint64_t address = 0;
        size_t pml4Index = PML4_GET_INDEX(addr), pdptIndex = PDPT_GET_INDEX(addr), pdirIndex = PDIR_GET_INDEX(addr), pageIndex = PAGE_GET_INDEX(addr);

        if(pml4Index == PDPTS_PER_PML4 - 1)
        {
            VmPageTable *pageTable = kernelPageTablePointers[pdptIndex][pdirIndex];

            if(pageTable != 0x0 && pageTable != NULL)
            {
                return (((*pageTable)[pageIndex].addr) * ARCH_PAGE_SIZE) + (addr & 0xFFF);
            }
        }
        else
        {

        }

        return addr;
    }

    PageEntry *GetVirtualPageDesc(VmPages *map, uintptr_t addr)
    {
        size_t pml4Index, pdptIndex, pdirIndex, pageIndex;

        pml4Index = PML4_GET_INDEX(addr);
        pdptIndex = PDPT_GET_INDEX(addr);
        pdirIndex = PDIR_GET_INDEX(addr);
        pageIndex = PAGE_GET_INDEX(addr);

        if(pml4Index == (PDPTS_PER_PML4 - 1))
        {
            // Kernel Address Space
            VmPageTable *pageTableEntry = kernelPageTablePointers[pdptIndex][pdirIndex]; //kernelPageDirs[pdptIndex].entries[pdirIndex];
            if (pageTableEntry != 0x0 && pageTableEntry != NULL)
                return &(*pageTableEntry)[pageIndex];
            else
                return NULL;
        }
        else
        {
            // Process Address Space
            return NULL;
        }
    }

    void LoadVirtualMemoryPages(VmPages *map)
    {
        asmi_load_paging(ConvertVirtToPhys(GetKernelPages(), 
                (uintptr_t) map
            ));
    }
} // namespace Memory