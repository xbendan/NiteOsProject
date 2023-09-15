#include <siberix/core/runtimes.h>

void*
operator new(unsigned long size)
{
    return (void*)siberix()->getMemory().alloc(size);
}

void*
operator new[](unsigned long size)
{
    return (void*)siberix()->getMemory().alloc(size);
}

void
operator delete(void* p)
{
    siberix()->getMemory().free((u64)p);
}

void
operator delete(void* p, unsigned long)
{
    ::operator delete(p);
}

void
operator delete[](void* p)
{
    siberix()->getMemory().free((u64)p);
}

void
operator delete[](void* p, unsigned long)
{
    ::operator delete[](p);
}

extern "C" void
__cxa_pure_virtual()
{
    while (true)
        asm volatile("cli; hlt");
}