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

    page_t *KernelAllocate4KPages(size_t amount) {
        amount = ALIGN_PAGE(amount);
        page_t *page = Memory::AllocatePhysMemory4K(PAGE_ORDER(amount));
        if(!page) {
            return nullptr;
        } else {
            uint64_t phys = page->addr, virt = ManagementUnit::KernelAllocate4KPages(amount);
            ManagementUnit::KernelMapVirutalMemory4K(phys, virt, amount);
            GetKernelProcess()->m_Pages += amount;
        }
    }

    page_t *Allocate4KPages(size_t amount) {
        amount = ALIGN_PAGE(amount);
        page_t *page = Memory::AllocatePhysMemory4K(PAGE_ORDER(amount));
        if (!page) {
            return nullptr;
        } else {
            proc_t *proc = GetCurrentProcess();
            uint64_t phys = page->addr, virt = ManagementUnit::Allocate4KPages(amount);
            ManagementUnit::MapVirtualMemory4K(proc->m_Pagemap, phys, virt, amount);
            proc->m_Pages += amount;
        }
    }

    void Free4KPages(uintptr_t address);
    void Free4KPages(page_t *page);
} // namespace Memory
 