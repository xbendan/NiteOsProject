#include <common/typedefs.h>

class AddressSpace {
public:
    AddressSpace() {}
    ~AddressSpace() {}

    u64 getAllocatedPages() { return m_allocatedPages; }
    u64 getMappedPages() { return m_mappedPages; }
    u64 getSwappedPages() { return m_swappedPages; }

    virtual u64  allocate4KPages(u64 amount)          = 0;
    virtual void free4KPages(u64 address, u64 amount) = 0;
    virtual void map(u64 phys, u64 virt, u64 amount)  = 0;
    virtual bool isPagePresent(u64 address)           = 0;
    virtual u64  convertVirtToPhys(u64 address)       = 0;

protected:
    u64   m_allocatedPages;
    u64   m_mappedPages;
    u64   m_swappedPages;
    u64 **m_bitmaps[512];
    u64   _zeroPage;
};
