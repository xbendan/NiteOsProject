#include <siberix/mem/kmem-alloc.h>

extern Kern::Mem::IMemAlloc* _kernMemAlloc;

void*
operator new(unsigned long size)
{
    return (void*)_kernMemAlloc->alloc(size);
}

void*
operator new[](unsigned long size)
{
    return (void*)_kernMemAlloc->alloc(size);
}

void
operator delete(void* p)
{
    _kernMemAlloc->free(p);
}

void
operator delete(void* p, unsigned long)
{
    _kernMemAlloc->free(p);
}

void
operator delete[](void* p)
{
    _kernMemAlloc->free(p);
}

void
operator delete[](void* p, unsigned long)
{
    _kernMemAlloc->free(p);
}