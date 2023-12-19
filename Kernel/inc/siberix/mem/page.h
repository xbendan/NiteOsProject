#include <stdcxx/types.h>

#define PAGE_SIZE_4K 4096
#define PAGE_SIZE_2M 2097152
#define PAGE_SIZE_1G 1073741824

namespace Kern::Mem {
    struct Page4K
    {
        UInt8 _pageOrder;
        UInt8 _flags;
        struct
        {
            UInt32 _inuse : 16;
            UInt32 _objects : 15;
            UInt32 _frozen : 1;
        } __attribute__((packed));
        union
        {
            UInt64  _priv;
            Page4K* _pageHead;
            Void*   _slabCache;
        };
    };

    inline UInt64 pageNumberOf(UInt64 address)
    {
        return address / PAGE_SIZE_4K;
    }

}