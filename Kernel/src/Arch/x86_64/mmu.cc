#include <Arch/x86_64/idt.h>
#include <Arch/x86_64/mmu.h>
#include <mm/mem.h>
#include <mm/kmalloc.h>
#include <mm/page.h>
#include <proc/sched.h>
#include <utils/spinlock.h>
#include <address.h>
#include <kern.h>

#define KERNEL_HEAP_PML4_INDEX 511
#define KERNEL_HEAP_PDPT_INDEX 511

namespace Memory::ManagementUnit
{
    pagemap_t   kernelPagemap;

    pml4_t      kernelPages __attribute__((aligned(ARCH_PAGE_SIZE)));
    pdpt_t      kernelPdpts __attribute__((aligned(ARCH_PAGE_SIZE)));
    pagedir_t   kernelPageDirs __attribute__((aligned(ARCH_PAGE_SIZE)));
    pagedir_t   kernelHeapPageDirs __attribute__((aligned(ARCH_PAGE_SIZE)));
    pagetable_t kernelHeapPageTables[TABLES_PER_DIR] __attribute__((aligned(ARCH_PAGE_SIZE)));

    pagedir_t   ioMappings[4] __attribute__((aligned(ARCH_PAGE_SIZE)));

    pagetable_t *kernelPageTablePointers[DIRS_PER_PDPT][TABLES_PER_DIR];

    void InitializeVirtualMemory() {
        memset(&kernelPages, 0, sizeof(pml4_t)); 
        memset(&kernelPdpts, 0, sizeof(pdpt_t)); 
        memset(&kernelPageDirs, 0, sizeof(pagedir_t)); 

        SetPageFrame(&(kernelPages[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)]), (uint64_t)&kernelPdpts - KERNEL_VIRTUAL_BASE, 0x3);
        kernelPages[0] = kernelPages[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)];

        SetPageFrame(&(kernelPdpts[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)]), (uint64_t)&kernelPageDirs - KERNEL_VIRTUAL_BASE, 0x3);
        for (int i = 0; i < TABLES_PER_DIR; i++) {
            SetPageFrame(&kernelPageDirs[i], (PAGE_SIZE_2M * i), 0x83);
        }

        SetPageFrame(&(kernelPdpts[KERNEL_HEAP_PDPT_INDEX]), (uint64_t)&kernelHeapPageDirs - KERNEL_VIRTUAL_BASE, 0x3);
        for (int j = 0; j < TABLES_PER_DIR; j++) {
            SetPageFrame(&(kernelHeapPageDirs[j]), (uint64_t)&(kernelHeapPageTables[j]) - KERNEL_VIRTUAL_BASE, 0x3);
            kernelPageTablePointers[KERNEL_HEAP_PDPT_INDEX][j] = &(kernelHeapPageTables[j]);
        }

        for (int i = 0; i < 4; i++) {
            SetPageFrame(&(kernelPdpts[PDPT_GET_INDEX(IO_VIRTUAL_BASE) + i]), ((address_t)&(ioMappings[i])) - KERNEL_VIRTUAL_BASE, 0x3);
            for (int j = 0; j < TABLES_PER_DIR; j++) {
                SetPageFrame(&ioMappings[i][j], (PAGE_SIZE_1G * i + PAGE_SIZE_2M * j), 0x83);
            }
        }
        kernelPdpts[0] = kernelPdpts[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)];

        asm("mov %%rax, %%cr3" ::"a"((address_t)&kernelPages - KERNEL_VIRTUAL_BASE));
    }

    bool IsPagePresent(pagemap_t *pagemap, uint64_t addr) {
        if (!pagemap)
            return false;

        size_t pml4Index = PML4_GET_INDEX(addr);
        size_t pdptIndex = PDPT_GET_INDEX(addr);
        size_t pdirIndex = PDIR_GET_INDEX(addr);
        size_t pageIndex = PAGE_GET_INDEX(addr);

        pml4e_t pml4Entry = pagemap->pml4[pml4Index];
        if (!(pml4Entry & PAGE_PRESENT)) {
            return false;
        }

        pdpte_t pdptEntry = pagemap->pdpts[pdptIndex];
        if (!(pdptEntry & PAGE_PRESENT)) {
            return false;
        }

        pde_t pdirEntry = (*(pagemap->pageDirs[pdptIndex]))[pdirIndex];
        if (pdirEntry & PAGE_PRESENT) {
            return (pdirEntry & PAGE_SIZE) ||
                (*(pagemap->pageTables[pdptIndex][pdirIndex])[pageIndex] & PAGE_PRESENT);
        }

        return false;
    }

    pagemap_t *CreatePagemap()
    {
        pagemap_t *pagemap = reinterpret_cast<pagemap_t *>(Memory::KernelAllocate4KPages(4));
        SetPageFrame(&(pagemap->pml4[0]), ConvertVirtToPhys(&kernelPagemap, (uint64_t)&pagemap->pdpts), 0x3);
        SetPageFrame(&(pagemap->pml4[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)]), (uint64_t)&kernelPdpts - KERNEL_VIRTUAL_BASE, 0x3);


        return pagemap;
    }

    void DestroyPagemap()
    {

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
    void MapVirtualMemory4K(pagemap_t *pagemap, uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags) {

    }

    /**
     * @brief 
     * 
     * @param amount 
     * @param pagemap 
     * @return uintptr_t 
     */
    uintptr_t Allocate4KPages(pagemap_t *pagemap, size_t amount) {
        return 0x0;
    }

    /**
     * @brief 
     * 
     * @param virt 
     * @param amount 
     * @param addressSpace 
     */
    void Free4KPages(pagemap_t *pagemap, uint64_t virt, size_t amount) {

    }
    
    void KernelMapVirtualMemory4K(uint64_t phys, uint64_t virt, size_t amount) {
        KernelMapVirtualMemory4K(phys, virt, amount, PAGE_PRESENT | PAGE_WRITABLE);
    }

    void KernelMapVirtualMemory4K(uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags)
    {
        size_t pdptIndex, pdirIndex, pageIndex;

        while (amount--) {
            pdptIndex = PDPT_GET_INDEX(virt);
            pdirIndex = PDIR_GET_INDEX(virt);
            pageIndex = PAGE_GET_INDEX(virt);

            pagetable_t *pageTable = kernelPageTablePointers[pdptIndex][pdirIndex];
            if (!pageTable) {
                uint64_t pageDirPhys = Memory::AllocatePhysMemory4K(1)->addr, pageDirVirt = KernelAllocate4KPages(1);
                KernelMapVirtualMemory4K(pageDirPhys, pageDirVirt, 1);
                uint64_t pageTablesPhys = Memory::AllocatePhysMemory4K(512)->addr, pageTablesVirt = KernelAllocate4KPages(512);
                KernelMapVirtualMemory4K(pageTablesPhys, pageTablesVirt, 512);

                SetPageFrame(&kernelPdpts[pdptIndex], pageDirPhys, 0x3);
                for (int i = 0; i < TABLES_PER_DIR; i++) {
                    SetPageFrame((uint64_t *) pageDirVirt + (i * sizeof(pde_t)), pageTablesPhys + (i * sizeof(pagetable_t)), 0x3);
                    kernelPageTablePointers[pdptIndex][i] = reinterpret_cast<pagetable_t *>(pageTablesVirt + (i * sizeof(pagetable_t)));
                }

                pageTable = kernelPageTablePointers[pdptIndex][pdirIndex];
            }

            SetPageFrame(&((*pageTable)[pageIndex]), phys, flags);

            phys += ARCH_PAGE_SIZE;
            virt += ARCH_PAGE_SIZE;
        }
    }

    /**
     * @brief 
     * 
     * @param amount 
     * @return uintptr_t
     */
    uintptr_t KernelAllocate4KPages(size_t amount) {
        uint64_t offset = 0, pageDirOffset = 0;
        uint64_t counter = 0;
        uintptr_t address;

        for (int i = 0; i < TABLES_PER_DIR; i++) {
            if((kernelHeapPageDirs[i] & PAGE_PRESENT) && !(kernelHeapPageDirs[i] & 0x80))  {
                for (int j = 0; j < PAGES_PER_TABLE; j++) {
                    if(kernelHeapPageTables[i][j] & PAGE_PRESENT) {
                        pageDirOffset = i;
                        offset = j + 1;
                        counter = 0;
                        continue;
                    }

                    counter++;

                    if(counter >= amount) {
                        address = ((pageDirOffset * PAGE_SIZE_2M) + (offset * PAGE_SIZE_4K)) | 0xFFFFFFFFC0000000;
                        while (counter--) {
                            if (offset >= 512) {
                                pageDirOffset++;
                                offset = 0;
                            }
                            SetPageFlags(&(kernelHeapPageTables[pageDirOffset][offset]), 0x3);
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
            if (!(kernelHeapPageDirs[i] & PAGE_PRESENT)) {
                counter += PAGES_PER_TABLE;

                if (counter >= amount) {
                    address = (PDPT_SIZE * KERNEL_HEAP_PML4_INDEX) + (PAGE_SIZE_1G * KERNEL_HEAP_PDPT_INDEX)
                        + (pageDirOffset * PAGE_SIZE_2M) + (offset * PAGE_SIZE_4K) | 0xFFFF000000000000;
                    SetPageFrame(&(kernelHeapPageDirs[pageDirOffset]), (uint64_t)&(kernelHeapPageTables[pageDirOffset]) - KERNEL_VIRTUAL_BASE, 0x3);
                    while (amount--) {
                        if (offset >= 512) {
                            pageDirOffset++;
                            offset = 0;
                            SetPageFrame(&(kernelHeapPageDirs[pageDirOffset]), (uint64_t)&(kernelHeapPageTables[pageDirOffset]) - KERNEL_VIRTUAL_BASE, 0x3);
                        }
                        SetPageFlags(&(kernelHeapPageTables[pageDirOffset][offset]), 0x3);
                        offset++;
                    }
                    return address;
                }
            } else {
                pageDirOffset = i + 1;
                offset = 0;
                counter = 0;
            }
        }

        System::Panic("Kernel Out of Virtual Memory!");
    }

    /**
     * @brief 
     * 
     * @param addr 
     * @param amount 
     */
    void KernelFree4KPages(void* addr, uint64_t amount) {

    }

    uintptr_t ConvertVirtToPhys(pagemap_t *pagemap, uintptr_t virt) {
        uint64_t phys = 0;
        size_t pml4Index = PML4_GET_INDEX(virt), pdptIndex = PDPT_GET_INDEX(virt), pdirIndex = PDIR_GET_INDEX(virt), pageIndex = PAGE_GET_INDEX(virt);

        if(pml4Index == PDPTS_PER_PML4 - 1 || pagemap == &kernelPagemap) {
            /* Kernel address space */
            if (pdptIndex == PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)) {
                return (PAGE_SIZE_2M * pdirIndex) + (virt % PAGE_SIZE_2M); 
            } else {
                pagetable_t *pageTable = kernelPageTablePointers[pdptIndex][pdirIndex];
                if (pageTable && ((*pageTable)[pageIndex] & PAGE_PRESENT)) {
                    return ((*pageTable)[pageIndex]) & PAGE_FRAME + (virt % ARCH_PAGE_SIZE);
                } else {
                    return 0x0;
                }
            }
        } else {
            /* Program address space, TODO */
            return 0x0;
        }
    }

    void HandlePageFault(void *ptr, registers_t *regs) {

    }
} // namespace Memory