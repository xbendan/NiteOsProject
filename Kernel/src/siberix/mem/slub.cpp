#include <siberix/kservice.h>
#include <siberix/mem/kmem-alloc.h>

namespace Kern::Mem {
    SlubAllocator::SlubAllocator() {}

    UInt64 SlubAllocator::alloc(UInt64 size)
    {
        return 0;
    }

    void SlubAllocator::free(void* address) {}
}