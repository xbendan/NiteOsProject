#include <Mem/KMemAlloc.h>
#include <macros>
#include <system.h>

inline void *operator new(size_t, void *p) { return p; }

void *operator new(size_t size) { return (void *) Memory::AllocateKernelObject(size); }

void *operator new[](size_t size) { return (void *) Memory::AllocateKernelObject(size); }

void operator delete(void* p) { Memory::FreeKernelObject((uintptr_t) p); }

void operator delete(void* p, size_t) { ::operator delete(p); }

void operator delete[](void* p) { Memory::FreeKernelObject((uintptr_t) p); }

void operator delete[](void* p, size_t) { ::operator delete[](p); }