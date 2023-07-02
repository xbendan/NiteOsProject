#include <siberix/mm/types.h>

namespace Memory
{
    class MemoryAlloc {
        virtual u64 alloc(u64 size) = 0;
        virtual void free(u64 address) = 0;
    };

    class PageAlloc {
        virtual Pageframe *allocatePhysMemory4K(u64 amount) = 0;
        virtual void freePhysMemory4K(u64 address) = 0;
        virtual void freePhysMemory4K(Pageframe *page) = 0;
    };

    class BuddyAlloc : public PageframeAllocator{
        Pageframe *allocatePhysMemory4K(u64 amount) override;
        void freePhysMemory4K(u64 address) override;
        void freePhysMemory4K(Pageframe *page) override;
        void markPagesUsed(u64 start, u64 end);
        Pageframe *expand(Pageframe *page);
        Pageframe *combine(Pageframe *page);
        void combine(Pageframe *lpage, Pageframe *rpage);

        inline bool checkAlignment(Pageframe *page) {
            return !((page->address) % ((1 << page->order) * PAGE_SIZE_4K));
        }
    }
} // namespace Memory