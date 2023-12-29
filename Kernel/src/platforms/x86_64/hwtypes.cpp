#include <arch-x86_64/hwtypes.h>
#include <siberix/main.h>
#include <siberix/mem/page.h>

namespace Kern::Platform::X64 {
    void TaskStateSegment::init(GdtPackage* pack)
    {
        pack->tss = GdtTssEntry(*this);
        for (int i = 0; i < 3; i++) {
            ist[i]  = Main::alloc4KPages(8);
            ist[i] += PAGE_SIZE_4K * 8;
        }
        asm volatile("mov %%rsp, %0" : "=r"(rsp[0]));
        asm volatile("ltr %%ax" ::"a"(0x28));
    }
}