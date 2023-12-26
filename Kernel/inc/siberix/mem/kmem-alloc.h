#include <siberix/mem/mem-alloc.h>
#include <stdcxx/array.h>
#include <stdcxx/string.h>

namespace Kern::Mem {
    class KernMemAlloc : public IMemAlloc
    {
    public:
        KernMemAlloc();
        ~KernMemAlloc() = delete;

        uint64_t alloc(uint64_t size) override;
        void     free(void* address) override;

    protected:
        /**
         * @brief The struct records the using status of cpu pages
         *
         * Each cpu has its own record, and can only access its own
         * page list for thread safety.
         */
        class MemoryPoolNode
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

        class MemoryPool
        {
        public:
            MemoryPool(Std::String<Utf8> name,
                       uint64_t          objSize,
                       uint64_t          flags = 0)
              : m_name(name)
              , m_objSize(objSize)
              , m_flags(flags)
            {
            }

        private:
            Std::String<Utf8>          m_name;
            uint64_t                   m_objSize;
            uint64_t                   m_flags;
            Std::Array<MemoryPoolNode> m_nodes;
        };

        template <typename T>
        class ObjectPool : MemoryPool
        {
        public:
            ObjectPool()
              : MemoryPool(sizeof(T))
            {
            }
        };

        Std::LinkedList<MemoryPool*> m_pools;
        uint32_t                     m_poolSizes[12];
    };
}