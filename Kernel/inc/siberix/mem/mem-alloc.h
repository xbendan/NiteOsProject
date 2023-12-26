#include <siberix/mem/page.h>
#include <stdcxx/linked-list.h>
#include <stdcxx/list.h>
#include <stdcxx/string.h>
#include <stdcxx/type-traits.h>
#include <stdcxx/types.h>
#include <xtra-math/align.h>

#define BUDDY_BOUND_UPPER 10
#define BUDDY_BOUND_LOWER 0
#define BUDDY_PFLAGS_FREE (1 << 0)
#define BUDDY_PFLAGS_KMEM (1 << 1)

namespace Kern::Mem {
    template <typename T>
        requires Std::IsIntegral<T> or Std::IsSame<T, Page4K*>
    T AllocatePhysMemory4K(uint64_t amount);

    template <typename T>
        requires Std::IsIntegral<T> or Std::IsSame<T, Page4K*>
    void FreePhysMemory4K(T waitForRelease);

    class IMemAlloc
    {
    public:
        virtual uint64_t alloc(uint64_t size) = 0;
        virtual void     free(void* address)  = 0;
    };
} // namespace siberix::mem