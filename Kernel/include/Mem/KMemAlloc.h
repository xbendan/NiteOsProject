#pragma once

#include <Mem/SlabAllocator.h>
#include <System.h>
#include <macros>

class KernelAllocator
{
public:
    virtual uintptr_t Allocate(size_t size) = 0;
    virtual void Free(uintptr_t address) = 0;
};

namespace Memory
{
    extern SlabAllocator *g_KernelAllocator;

    void SlubInit();
    uintptr_t AllocateKernelObject(uint32_t size);
    void FreeKernelObject(uintptr_t addr);
} // namespace Memory