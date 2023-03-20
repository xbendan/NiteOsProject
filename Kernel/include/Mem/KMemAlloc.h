#pragma once

#include <System.h>
#include <macros>

namespace Memory
{
    void SlubInit();
    uintptr_t AllocateKernelObject(uint32_t size);
    void FreeKernelObject(uintptr_t addr);
} // namespace Memory

extern "C" {
    static inline uintptr_t kmalloc(size_t size) { return Memory::AllocateKernelObject(size); }
    static inline void kfree(void *ptr) { Memory::FreeKernelObject((uintptr_t) ptr); }
}