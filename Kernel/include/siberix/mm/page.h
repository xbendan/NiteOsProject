#include <utils/

namespace mm {
    struct page {
        uint8_t order;
        uint8_t flags;
        struct {
            uint32_t slab_inuse : 16;
            uint32_t slab_objects : 15;
            uint32_t slab_frozon : 1;
        } __attribute__((packed));
        union {
            uint64_t priv;
            void *slab_cache;
            struct page *first;
        };
        void **freelist;
    }
}