#include <init/bootinfo.h>
#include <mm/mem.h>
#include <mm/page.h>
#include <mm/slab.h>
#include <mm/mmzone.h>
#include <drv/video.h>
#include <proc/sched.h>
#include <kern.h>
#include <address.h>

#ifdef ARCH_X86_64
#include <Arch/x86_64/mmu.h>
#endif

namespace Memory
{
    /// @brief memory usage statistics
    /// total, available, present, swapped, 
    uint64_t stat[8];

    void Initialize() {
        bootmem_t *mem = &bootInfo.m_Memory;

        stat[MEMVAL_TOTAL] = mem->m_TotalSize;
        stat[MEMVAL_AVAILABLE] = mem->m_Usable;

        ManagementUnit::InitializeVirtualMemory();

        Model::MemblocksInit(mem);
        Model::MemsectsInit();

        BuddyInit();        
        KmallocInit();
    }
} // namespace Memory