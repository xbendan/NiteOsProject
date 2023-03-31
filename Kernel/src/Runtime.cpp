#include <Mem/KMemAlloc.h>
#include <macros>
#include <System.h>

void *operator new(size_t size) { return (void *) Memory::g_KernelAllocator->Allocate(size); }

void *operator new[](size_t size) { return (void *) Memory::g_KernelAllocator->Allocate(size); }

void operator delete(void* p) { Memory::g_KernelAllocator->Free((uintptr_t) p); }

void operator delete(void* p, size_t) { ::operator delete(p); }

void operator delete[](void* p) { Memory::g_KernelAllocator->Free((uintptr_t) p); }

void operator delete[](void* p, size_t) { ::operator delete[](p); }