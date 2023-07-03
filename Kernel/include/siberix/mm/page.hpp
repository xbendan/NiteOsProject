#include <siberix/mm/types.h>
#include <utils/array.h>
#include <utils/linked_list.h>
#include <utils/spinlock.h>

#define PAGES_PER_SECTION 262144
#define PAGE_MAX_ORDER 10
#define PAGE_MIN_ORDER 0

namespace Memory
{
    class MemoryAlloc {
        virtual u64 alloc(u64 size) = 0;
        virtual void free(u64 address) = 0;
    };

    class PageAlloc {
        virtual u64 allocatePhysMemory4K(u64 amount) = 0;
        virtual void freePhysMemory4K(u64 address) = 0;
        virtual void freePhysMemory4K(Pageframe* page) = 0;
    };

    class SegAlloc : public PageAlloc {
    public:
        SegAlloc();
        ~SegAlloc();

        Pageframe* allocatePhysMemory4K(u64 amount) override;
        void freePhysMemory4K(u64 address) override;
        void freePhysMemory4K(Pageframe* page) override;
        void addSegment(u64 start, u64 end, PageBlockType type);
        SizedArrayList<PageBlock, 256>* getSegments();

    private:
        SizedArrayList<PageBlock, 256> segments;
    };

    class BuddyAlloc : public PageframeAllocator {
    public:
        BuddyAlloc();
        ~BuddyAlloc();

        Pageframe* allocatePhysMemory4K(u64 amount) override;
        void freePhysMemory4K(u64 address) override;
        void freePhysMemory4K(Pageframe& page) override;
        void markPagesUsed(u64 addressStart, u64 addressEnd);
        Pageframe* expand(Pageframe& page);
        Pageframe* combine(Pageframe& page);
        Pageframe* combine(Pageframe& lpage, Pageframe& rpage);

        static inline bool checkAlignment(Pageframe* page) {
            return !((page->address) % ((1 << page->order) * PAGE_SIZE_4K));
        }
        static inline u64 getPageAlignment(u64 x) {
            x--;
            x |= x >> 1;
            x |= x >> 2;
            x |= x >> 4;
            x |= x >> 8;
            x |= x >> 16;
            return x += 1;
        }
        static inline u8 getPageOrder(u64 size) {
            u8 order = PAGE_MAX_ORDER;
            u64 _size = 1024;
            while (_size > size)
            {
                _size /= 2;
                order--;
            }
            return order;
        }

    private:
        /**
         * This array contains the areas struct
         * The lowest is 0, equals to 4KiB (1 page)
         * The highest is 10, equals to 4MiB (1024 pages)
         */
        LinkedList<Pageframe> pageList[PAGE_MAX_ORDER + 1];
        u64 flags;
        spinlock_t lock;
    };
} // namespace Memory