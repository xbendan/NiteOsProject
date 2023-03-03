#include <mm/kmalloc.h>
#include <macros>

void *operator new(size_t size) { return Memory::AllocateKernelObject(size); }

void* operator new(size_t size) { return Memory::AllocateKernelObject(size); }

void* operator new[](size_t size) { return Memory::AllocateKernelObject(size); }

void operator delete(void* p) { Memory::FreeKernelObject(p); }

void operator delete(void* p, size_t) { ::operator delete(p); }

void operator delete[](void* p) { Memory::FreeKernelObject(p); }

void operator delete[](void* p, size_t) { ::operator delete[](p); }