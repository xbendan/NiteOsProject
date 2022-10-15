#include <Init/BootInfo.h>
#include <Memory/Memory.h>
#include <Memory/Page.h>
#include <Memory/Slub.h>
#include <Memory/Address>
#include <graphic/terminal.h>
#include <Process/Scheduler.h>
#include <Kernel>

#ifdef ARCH_X86_64
#include <Arch/x86_64/Paging.h>
#endif

namespace Memory
{
    MemoryStatistic memStats;

    void Initialize()
    {
        LoadVirtualMemory();
        LoadPageAllocator();

        MapVirtualAddress(
            GetKernelPages(), 
            0x00000000, 
            KERNEL_LOW_4GB, 
            0x100000, 
            PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE
        );
    }
} // namespace Memory