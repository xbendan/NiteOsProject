#include <common/typedefs.h>

class AddressSpace {
public:
    AddressSpace() {}
    ~AddressSpace() {}

    u64 getAllocatedPages() { return allocatedPages; }
    u64 getMappedPages() { return mappedPages; }
    u64 getSwappedPages() { return swappedPages; }

    virtual u64 allocate4KPages(u64 amount);
    virtual void free4KPages(u64 address, u64 amount);
    virtual bool isPagePresent(u64 address);
    virtual u64 convertVirtToPhys(u64 address);

protected:
    u64 allocatedPages;
    u64 mappedPages;
    u64 swappedPages;
    u64 _zeroPage;
};
