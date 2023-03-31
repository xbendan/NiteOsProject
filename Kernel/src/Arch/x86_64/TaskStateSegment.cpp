#include <Arch/x86_64/CPU.h>
#include <Mem/Page.h>

extern "C" void load_tss(uint64_t addr, uint64_t gdt, uint8_t selector);

namespace TSS {
    void Initialize(tss_t* tss, void* gdt) {
        // load_tss((uintptr_t)tss, (uint64_t)gdt, 0x28);
        GDTPackage *package = reinterpret_cast<GDTPackage *>(gdt);
        package->tss = GDTTssEntry(*tss);

        memset(tss, 0, sizeof(tss_t));

        // Set up Interrupt Stack Tables
        for (int i = 0; i < 3; i++) {
            tss->ist[i] = (uint64_t) Memory::KernelAllocate4KPages(8);
            memset((void *) tss->ist[i], 0, ARCH_PAGE_SIZE);
            tss->ist[i] += ARCH_PAGE_SIZE * 8;
        }
        
        asm volatile("mov %%rsp, %0" : "=r"(tss->rsp[0]));
        asm volatile("ltr %%ax" ::"a"(0x28));
    }
} // namespace TSS
