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
    class MemoryAllocatorBase
    {
        virtual UInt64 alloc(UInt64 size)  = 0;
        virtual void   free(void* address) = 0;
    };

    class PagedAllocatorBase : public MemoryAllocatorBase
    {
    public:
        PagedAllocatorBase();
        ~PagedAllocatorBase();

        UInt64 alloc(UInt64 size) override
        {
            return allocPhysMemory4K<UInt64>(
              Xtra::Math::alignUp(size, PAGE_SIZE_4K) / PAGE_SIZE_4K);
        }

        void free(void* address) override
        {
            freePhysMemory4K(reinterpret_cast<UInt64>(address));
        }

        template <typename T>
            requires Std::IsIntegral<T> or Std::IsSame<T, Page4K*>
        T allocPhysMemory4K(UInt64 amount);

        template <typename T>
            requires Std::IsIntegral<T> or Std::IsSame<T, Page4K*>
        void freePhysMemory4K(T waitForRelease);

        Page4K* pageExpand(Page4K* page)
        {
            if (page->_flags & BUDDY_PFLAGS_KMEM) {
                return nullptr;
            }

            m_freePages[page->_order].remove(page);
            Std::LinkedList<Page4K>& pageList = m_freePages[--page->_order];
        }

    private:
        /**
         * This array contains the areas struct
         * The lowest is 0, equals to 4KiB (1 page)
         * The highest is 10, equals to 4MiB (1024 pages)
         */
        Std::LinkedList<Page4K> m_freePages[BUDDY_BOUND_UPPER + 1];
    };

} // namespace siberix::mem