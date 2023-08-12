#include <siberix/core/runtimes.h>

void* operator new(unsigned long size) {
    return (void*)exec()->getMemory().alloc(size);
}

void* operator new[](unsigned long size) {
    return (void*)exec()->getMemory().alloc(size);
}

void operator delete(void* p) { exec()->getMemory().free((u64)p); }

void operator delete(void* p, unsigned long) { ::operator delete(p); }

void operator delete[](void* p) { exec()->getMemory().free((u64)p); }

void operator delete[](void* p, unsigned long) { ::operator delete[](p); }