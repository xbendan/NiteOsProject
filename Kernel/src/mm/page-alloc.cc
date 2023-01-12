#include <mm/page.h>
#include <proc/proc.h>
#include <proc/sched.h>
#include <macros>

#ifdef ARCH_X86_64
#include <Arch/x86_64/mmu.h>
#endif

namespace Memory
{
    using namespace Proc;

    uintptr_t KernelAllocate4KPages(size_t amount) {
        amount = ALIGN_PAGE(amount);
        page_t *page = Memory::AllocatePhysMemory4K(amount);
        if(!page) {
            return 0x0;
        } else {
            uint64_t phys = page->addr, virt = ManagementUnit::KernelAllocate4KPages(amount);
            ManagementUnit::KernelMapVirtualMemory4K(phys, virt, amount);
            GetKernelProcess()->m_Pages += amount;
            return virt;
        }
    }

    uintptr_t Allocate4KPages(size_t amount) {
        amount = ALIGN_PAGE(amount);
        page_t *page = Memory::AllocatePhysMemory4K(amount);
        if (!page) {
            return 0x0;
        } else {
            proc_t *proc = GetCurrentProcess();
            uint64_t phys = page->addr, virt = ManagementUnit::Allocate4KPages(proc->m_Pagemap, amount);
            ManagementUnit::MapVirtualMemory4K(proc->m_Pagemap, phys, virt, amount);
            proc->m_Pages += amount;
            return virt;
        }
    }

    void Free4KPages(uintptr_t address);
    void Free4KPages(page_t *page);
} // namespace Memory
 