#include <Mem/Memory.h>
#include <Mem/KMemAlloc.h>
#include <Mem/Page.h>
#include <Mem/MMIO.h>
#include <Mem/AddressSpace.h>
#include <Proc/Scheduler.h>
#include <Utils/Spinlock.h>
#include <System.h>

#include <Arch/x86_64/MMU.h>
#include <Arch/x86_64/Interrupts.h>

#define KERNEL_HEAP_PML4_INDEX 511
#define KERNEL_HEAP_PDPT_INDEX 511

namespace Paging
{
    VirtualPages g_KernelPagemap;
    KernelAddressSpace g_KernelSpace = KernelAddressSpace();

    pml4_t      kernelPages __attribute__((aligned(ARCH_PAGE_SIZE)));
    pdpt_t      kernelPdpts __attribute__((aligned(ARCH_PAGE_SIZE)));
    pagedir_t   kernelPageDirs __attribute__((aligned(ARCH_PAGE_SIZE)));
    pagedir_t   kernelHeapPageDirs __attribute__((aligned(ARCH_PAGE_SIZE)));
    pagetable_t kernelHeapPageTables[TABLES_PER_DIR] __attribute__((aligned(ARCH_PAGE_SIZE)));

    pagedir_t   ioMappings[4] __attribute__((aligned(ARCH_PAGE_SIZE)));

    pagetable_t *kernelPageTablePointers[DIRS_PER_PDPT][TABLES_PER_DIR];

    void PageFaultHandler(InterruptData *data, RegisterContext *context)
    {
        
    }

    void InitializeVirtualMemory() {
        // memset(&kernelPages, 0, sizeof(pml4_t)); 
        // memset(&kernelPdpts, 0, sizeof(pdpt_t)); 
        // memset(&kernelPageDirs, 0, sizeof(pagedir_t)); 

        // SetPageFrame(&(kernelPages[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)]), (uint64_t)&kernelPdpts - KERNEL_VIRTUAL_BASE, 0x3);
        // kernelPages[0] = kernelPages[PML4_GET_INDEX(KERNEL_VIRTUAL_BASE)];

        // SetPageFrame(&(kernelPdpts[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)]), (uint64_t)&kernelPageDirs - KERNEL_VIRTUAL_BASE, 0x3);
        // for (int i = 0; i < TABLES_PER_DIR; i++) {
        //     SetPageFrame(&kernelPageDirs[i], (PAGE_SIZE_2M * i), 0x83);
        // }

        // SetPageFrame(&(kernelPdpts[KERNEL_HEAP_PDPT_INDEX]), (uint64_t)&kernelHeapPageDirs - KERNEL_VIRTUAL_BASE, 0x3);
        // for (int j = 0; j < TABLES_PER_DIR; j++) {
        //     SetPageFrame(&(kernelHeapPageDirs[j]), (uint64_t)&(kernelHeapPageTables[j]) - KERNEL_VIRTUAL_BASE, 0x3);
        //     kernelPageTablePointers[KERNEL_HEAP_PDPT_INDEX][j] = &(kernelHeapPageTables[j]);
        // }

        // for (int i = 0; i < 4; i++) {
        //     SetPageFrame(&(kernelPdpts[PDPT_GET_INDEX(IO_VIRTUAL_BASE) + i]), ((uint64_t)&(ioMappings[i])) - KERNEL_VIRTUAL_BASE, 0x3);
        //     for (int j = 0; j < TABLES_PER_DIR; j++) {
        //         SetPageFrame(&ioMappings[i][j], (PAGE_SIZE_1G * i + PAGE_SIZE_2M * j), (PAGE_PRESENT | PAGE_WRITABLE | PAGE_SIZE | PAGE_NOCACHE));
        //     }
        // }
        // kernelPdpts[0] = kernelPdpts[PDPT_GET_INDEX(KERNEL_VIRTUAL_BASE)];

        g_KernelSpace = KernelAddressSpace(&g_KernelPagemap);
        Task::g_KernelProcess.m_AddressSpace = &g_KernelSpace;

        asm("mov %%rax, %%cr3" ::"a"(g_KernelPagemap.pml4Phys));
    }

    VirtualPages *CreatePagemap()
    {

    }

    void DestroyPagemap()
    {

    }

    void CreatePageTable(uint64_t entry)
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
    void MapVirtualMemory4K(VirtualPages *pagemap, uint64_t phys, uint64_t virt, size_t amount, page_flags_t flags) {

    }

    /**
     * @brief 
     * 
     * @param amount 
     * @param pagemap 
     * @return uintptr_t 
     */
    uintptr_t Allocate4KPages(VirtualPages *pagemap, size_t amount) {
        return 0x0;
    }

    /**
     * @brief 
     * 
     * @param virt 
     * @param amount 
     * @param addressSpace 
     */
    void Free4KPages(VirtualPages *pagemap, uint64_t virt, size_t amount) {

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
                System::Out("No page table present.");
                uint64_t pageDirPhys = Memory::AllocatePhysMemory4K(1)->address, pageDirVirt = KernelAllocate4KPages(1);
                KernelMapVirtualMemory4K(pageDirPhys, pageDirVirt, 1);
                uint64_t pageTablesPhys = Memory::AllocatePhysMemory4K(512)->address, pageTablesVirt = KernelAllocate4KPages(512);
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
                    address = ((PDPT_SIZE * KERNEL_HEAP_PML4_INDEX) + (PAGE_SIZE_1G * KERNEL_HEAP_PDPT_INDEX)
                        + (pageDirOffset * PAGE_SIZE_2M) + (offset * PAGE_SIZE_4K)) | 0xFFFF000000000000;
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

    uintptr_t ConvertVirtToPhys(VirtualPages *pagemap, uintptr_t virt) {
        size_t pml4Index = PML4_GET_INDEX(virt), pdptIndex = PDPT_GET_INDEX(virt), pdirIndex = PDIR_GET_INDEX(virt), pageIndex = PAGE_GET_INDEX(virt);

        if(pml4Index == PDPTS_PER_PML4 - 1 || pagemap == &g_KernelPagemap) {
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
    
    using namespace Memory;

    void VirtualPages::CheckBitmap(uint64_t d, uint64_t t)
    {
        if (!bitmaps[d][t])
        {
            uint64_t base = ALIGN_DOWN(t, 64);
            uint64_t address = Task::g_KernelProcess.m_AddressSpace->Allocate4KPages(1);
            for (int i = 0; i < 64; i++)
            {
                bitmaps[d][base + i] = (uint64_t *)(address + (i * 64));
            }
        }
    }

    void VirtualPages::CheckPageTable(uint64_t d, uint64_t t)
    {
        AddressSpace *kernelSpace = Task::g_KernelProcess.m_AddressSpace;

        if (!(pdpts[d] & PagePresent) || !pageDirs[d])
        {
            uint64_t phys;
            pageDirs[d] = (pde_t *) kernelSpace->Allocate4KPages(1);
            pageTables[d] = (page_t **) kernelSpace->Allocate4KPages(1);
            phys = kernelSpace->ConvertVirtToPhys((uint64_t) pageDirs[d]);

            SetPageFrame(&pdpts[d], phys, (PagePresent | PageWritable | PageUser));
        }

        if (!(pageDirs[d][t] & PagePresent) || !pageTables[d][t])
        {
            uint64_t phys;
            pageTables[d][t] = (page_t *) Task::g_KernelProcess.m_AddressSpace->Allocate4KPages(1);
            phys = kernelSpace->ConvertVirtToPhys((uint64_t) pageTables[d][t]);

            SetPageFrame(&pageDirs[d][t], phys, (PagePresent | PageWritable | PageUser));
        }
    }
} // namespace Memory

namespace Memory
{
    uint64_t GetIOMapping(uint64_t address) {
        return Paging::GetIOMapping(address);
    }
}