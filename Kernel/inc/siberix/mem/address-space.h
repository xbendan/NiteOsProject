#include <stdcxx/types.h>

namespace Kern::Mem {
    class AddressSpace
    {
    public:
        AddressSpace()
          : m_bound(0)
          , m_allocatedPages(0)
          , m_mappedPages(0)
          , m_swappedPages(0)
          , m_zeroPage(nullptr)
        {
        }
        ~AddressSpace() { delete m_zeroPage; }

        virtual UInt64  alloc4KPages(UInt64 amount)                  = 0;
        virtual Void    free4KPages(UInt64 address, UInt64 amount)   = 0;
        virtual Void    map(UInt64 phys, UInt64 virt, UInt64 amount) = 0;
        virtual Boolean isPagePresent(UInt64 address)                = 0;
        virtual UInt64  convertVirtToPhys(UInt64 address)            = 0;

        virtual Void load() = 0;

        UInt64 getAllocatedPages() const { return m_allocatedPages; }
        UInt64 getMappedPages() const { return m_mappedPages; }
        UInt64 getSwappedPages() const { return m_swappedPages; }

    protected:
        UInt64 m_bound;
        /*
            The allocated pages indicates how many pages were asked to
           allocate
         */
        UInt64 m_allocatedPages;
        /*
            The mapped pages is simply the allocated pages exclude the pages
            that is temporarily mapped to the zero page.
         */
        UInt64 m_mappedPages;
        UInt64 m_swappedPages;
        /*
            This zero page is neither not a page full of zeros, nor a page that
            mapped to the zero address. It should be considered as a temporary
            page that all virtual allocations should point to this page until
            they are factually accessed. This page is used to prevent the
            kernel from allocating a lot of pages that are not used.
         */
        Void*  m_zeroPage;
    };
}