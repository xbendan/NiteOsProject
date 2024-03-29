#include <siberix/mem/mem-alloc.h>
#include <stdcxx/array.h>
#include <stdcxx/string.h>

namespace Kern::Mem {
    class KernMemAlloc : public IMemAlloc
    {
    public:
        KernMemAlloc();
        ~KernMemAlloc() = default;

        uint64_t alloc(uint64_t size) override;
        void     free(void* address) override;

    protected:
        /**
         * @brief The struct records the using status of cpu pages
         *
         * Each cpu has its own record, and can only access its own
         * page list for thread safety.
         */
        class MemPoolNode
        {
        public:
            void**                   _freelist;
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

        class MemPool
        {
        public:
            MemPool(Std::String<Utf8> name,
                    uint64_t          objSize,
                    uint64_t          flags = 0)
              : m_name(name)
              , m_objSize(objSize)
              , m_flags(flags)
            {
            }
            ~MemPool() = default;

        private:
            Std::String<Utf8>      m_name;
            uint64_t               m_objSize;
            uint64_t               m_flags;
            Array<MemPoolNode, 12> m_nodes;
        };

        template <typename T>
        class ObjectPool : MemPool
        {
        public:
            ObjectPool()
              : MemPool(sizeof(T))
            {
            }
        };

        Array<MemPool*, 12> m_pools;
        Array<uint32_t, 12> m_poolSizes;
    };
}