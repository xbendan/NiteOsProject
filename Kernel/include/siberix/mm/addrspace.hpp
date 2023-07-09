#include <common/typedefs.h>

class AddressSpace {
public:
    AddressSpace() {}
    ~AddressSpace() {}

    u64 getAllocatedPages() { return allocatedPages; }
    u64 getMappedPages() { return mappedPages; }
    u64 getSwappedPages() { return swappedPages; }

    virtual u64 allocate4KPages(u64 amount) = 0;
    virtual void free4KPages(u64 address, u64 amount) = 0;
    virtual bool isPagePresent(u64 address) = 0;
    virtual u64 convertVirtToPhys(u64 address) = 0;

protected:
    u64 allocatedPages;
    u64 mappedPages;
    u64 swappedPages;
    u64 _zeroPage;
};
