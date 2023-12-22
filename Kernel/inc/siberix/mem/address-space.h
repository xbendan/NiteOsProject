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

        /**
         * @brief Allocate virtual 4K pages
         *
         * @param amount
         * @param writable whether the page is writable
         * @param directMap2M allow the page directory entry to directly map to
         * physical address when the remained amount is over 512 pages.
         * @return uint64_t the virtual address of the allocated pages
         */
        virtual uint64_t  alloc4KPages(uint64_t  amount,
                                     bool isWritable      = true,
                                     bool isWriteThrough  = false,
                                     bool isCacheDisabled = false,
                                     bool directMap2M     = true)     = 0;
        virtual void    free4KPages(uint64_t address, uint64_t amount)   = 0;
        virtual void    map(uint64_t phys, uint64_t virt, uint64_t amount) = 0;
        virtual bool isPagePresent(uint64_t address)                = 0;
        virtual uint64_t  convertVirtToPhys(uint64_t address)            = 0;

        virtual void load() = 0;

        uint64_t getAllocatedPages() const { return m_allocatedPages; }
        uint64_t getMappedPages() const { return m_mappedPages; }
        uint64_t getSwappedPages() const { return m_swappedPages; }

    protected:
        uint64_t m_bound;
        /*
            The allocated pages indicates how many pages were asked to
           allocate
         */
        uint64_t m_allocatedPages;
        /*
            The mapped pages is simply the allocated pages exclude the pages
            that is temporarily mapped to the zero page.
         */
        uint64_t m_mappedPages;
        uint64_t m_swappedPages;
        /*
            This zero page is neither not a page full of zeros, nor a page that
            mapped to the zero address. It should be considered as a temporary
            page that all virtual allocations should point to this page until
            they are factually accessed. This page is used to prevent the
            kernel from allocating a lot of pages that are not used.
         */
        void*  m_zeroPage;
    };
}