#include <stdcxx/linked-list.h>
#include <stdcxx/types.h>

#define PAGE_SIZE_4K 4096ULL
#define PAGE_SIZE_2M 2097152ULL
#define PAGE_SIZE_1G 1073741824ULL

namespace Kern::Mem {
    struct Page4K : Std::LinkedList<Page4K>::Entry
    {
        UInt8 _order;
        UInt8 _flags;
        struct /* slub */
        {
            struct
            {
                UInt32 _inuse : 16;
                UInt32 _objects : 15;
                UInt32 _frozen : 1;
            } __attribute__((packed));
            Void** _freelist;
        };

        union
        {
            UInt64  _priv;
            Page4K* _pageHead;
            Void*   _kmemPool;
        };
        UInt64 _address;
    };

}