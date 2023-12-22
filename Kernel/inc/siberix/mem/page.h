#include <stdcxx/linked-list.h>
#include <stdcxx/types.h>

#define PAGE_SIZE_4K 4096ULL
#define PAGE_SIZE_2M 2097152ULL
#define PAGE_SIZE_1G 1073741824ULL
#define PAGE_FLAG_FREE (1 << 0)
#define PAGE_FLAG_KMEM (1 << 1)
#define PAGE_FLAG_SWAP (1 << 2)
#define PAGE_FLAG_LOCK (1 << 3)
#define PAGE_FLAG_DIRTY (1 << 4)
#define PAGE_FLAG_SHARE (1 << 5)

namespace Kern::Mem {
    struct Page4K : Std::LinkedList<Page4K>::Entry
    {
        uint8_t _order;
        uint8_t _flags;
        struct /* slub */
        {
            struct
            {
                uint32_t _inuse : 16;
                uint32_t _objects : 15;
                uint32_t _frozen : 1;
            } __attribute__((packed));
            void** _freelist;
        };

        union
        {
            uint64_t _priv;
            Page4K*  _pageHead;
            void*    _kmemPool;
        };
        uint64_t _address;

        inline uint64_t offset()
        {
            return (1 << _order) * sizeof(Page4K) * PAGE_SIZE_4K;
        }

        inline bool isLeftPage() { return !(_address % offset()); }

        constexpr Page4K* split()
        {
            if (!(_flags & PAGE_FLAG_FREE)) {
                return nullptr;
            }

            uint64_t offset   = ((1 << --_order) * sizeof(Page4K));
            Page4K*  newPage  = reinterpret_cast<Page4K*>(this + offset);
            newPage->_order   = _order;
            newPage->_flags   = 0;
            newPage->_address = _address + offset * PAGE_SIZE_4K;

            return newPage;
        }

        constexpr Page4K* merge(Page4K* page)
        {
            if (!(_flags & PAGE_FLAG_FREE && page->_flags & PAGE_FLAG_FREE)) {
                return nullptr;
            }

            Page4K* newPage = reinterpret_cast<Page4K*>(
              isLeftPage() ? this + offset() : this - offset());

            Page4K* result = isLeftPage() ? this : newPage;
            result->_order++;

            return result;
        }
    };
}