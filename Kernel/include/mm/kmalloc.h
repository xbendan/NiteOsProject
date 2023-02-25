#include <macros>

extern "C" {
    uintptr_t kmalloc(size_t size);
    void kfree(void *ptr);

    uint64_t kallocpg(size_t amount);
    void kfreepg(uintptr_t addr);
    uint64_t allocpg(size_t amount);
    void freepg(uintptr_t addr);
}

namespace Memory
{
    void KmallocInit();
    uintptr_t KernelMemoryAllocate(uint32_t size);
    void KernelMemoryFree(uintptr_t addr);
} // namespace Memory
