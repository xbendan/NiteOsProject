#include <Mem/Page.h>
#include <Mem/KMemAlloc.h>
#include <Proc/Process.h>
#include <Proc/Scheduler.h>
#include <macros>
#include <kern.h>

#ifdef ARCH_X86_64
#include <Arch/x86_64/mmu.h>
#endif

namespace Memory
{
    using namespace Task;

    uintptr_t KernelAllocate4KPages(size_t amount) {
        amount = ALIGN_PAGE(amount);
        page_t *page = Memory::AllocatePhysMemory4K(amount);

        if (page != nullptr) {
            uint64_t phys = page->addr;
            uint64_t virt = ManagementUnit::KernelAllocate4KPages(amount);
            ManagementUnit::KernelMapVirtualMemory4K(phys, virt, amount);
            return virt;
        }

        return 0x0;
    }

    uintptr_t Allocate4KPages(size_t amount) {
        amount = ALIGN_PAGE(amount);
        page_t *page = Memory::AllocatePhysMemory4K(amount);

        if (page != nullptr) {
            Process *proc = GetCurrentProcess();
            uint64_t phys = page->addr;
            uint64_t virt = ManagementUnit::Allocate4KPages(proc->m_Pagemap, amount);

            ManagementUnit::MapVirtualMemory4K(proc->m_Pagemap, phys, virt, amount);
            proc->m_Pages += amount;

            return virt;
        }

        return 0x0;
    }

    void Free4KPages(uintptr_t address) {

    }

    void Free4KPages(page_t *page) {

    }
} // namespace Memory
 