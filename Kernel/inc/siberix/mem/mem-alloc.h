#include <siberix/mem/page.h>
#include <stdcxx/concepts.h>
#include <stdcxx/linked-list.h>
#include <stdcxx/list.h>
#include <stdcxx/string.h>
#include <stdcxx/types.h>
#include <xtra-math/align.h>

namespace Kern::Mem {
    class IMemoryAlloc
    {
        virtual UInt64 alloc(UInt64 size)  = 0;
        virtual void   free(void* address) = 0;
    };

    class IPagedMemoryAlloc : public IMemoryAlloc
    {
        UInt64 alloc(UInt64 size) override
        {
            return allocPhysMemory4K<UInt64>(
              Xtra::Math::alignTwoExponent(size) / PAGE_SIZE_4K);
        }

        void free(void* address) override
        {
            freePhysMemory4K(reinterpret_cast<UInt64>(address));
        }

        template <typename T>
            requires Std::Integral<T> or Std::Same<T, Page4K*>
        T allocPhysMemory4K(UInt64 amount);

        void freePhysMemory4K(UInt64 address);

        void freePhysMemory4K(Page4K* page);
    };

    class LwSlabAlloc : public IMemoryAlloc
    {
    public:
        UInt64 alloc(UInt64 size) override;
        void   free(void* address) override;

    private:
        /**
         * @brief The struct records the using status of cpu pages
         *
         * Each cpu has its own record, and can only access its own
         * page list for thread safety.
         */
        struct KMemPages
        {
            /**
             * @brief The page that is currently in used.
             *
             * Once all the space in this page is used, it will be
             * removed.
             */
            Page4K*                  _page;
            /**
             * @brief The list contains all pages that are not fully used
             *
             * Usually the page is fullfiled with assigned objects and it
             * will be removed, but once an object inside a full page was
             * released, it will be added to this list and wait for the
             * next allocation.
             */
            Std::LinkedList<Page4K*> _partial;
        };

        struct KMemCache
        {
            Std::String<Utf8>           m_name;
            Std::LinkedList<KMemPages*> m_perCpuPages;
            UInt32                      m_size; // The size of each object
            UInt64                      m_flags;
            UInt32                      m_offset;

            KMemCache(Std::String<Utf8> name, UInt64 size, UInt64 flags)
              : m_name(name)
              , m_size(size)
              , m_flags(flags)
            {
            }
        };
        Std::LinkedList<KMemCache*>* _slabCaches;
    };
} // namespace siberix::mem