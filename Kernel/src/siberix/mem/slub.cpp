#include <siberix/kservice.h>
#include <siberix/mem/kmem-alloc.h>

namespace Kern::Mem {
    SlubAllocator::SlubAllocator() {}

    uint64_t SlubAllocator::alloc(uint64_t size)
    {
        return 0;
    }

    void SlubAllocator::free(void* address) {}
}