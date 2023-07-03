#include <siberix/mm/page.hpp>

class MemoryService {
public:
    MemoryService();
    ~MemoryService();
    /// @brief
    /// @param amount
    /// @return
    u64 alloc4KPages(u64 amount);
    /// @brief
    /// @param address
    void free4KPages(u64 address);

    /// @brief
    /// @param amount
    /// @return
    Pageframe* allocPhysMemory4KPages(u64 amount);
    /// @brief
    /// @param amount
    /// @return
    u64 allocPhysMemory4K(u64 amount);
    /// @brief
    /// @param address
    void freePhysMemory4KPages(u64 address);
    /// @brief
    /// @param page
    void freePhysMemory4KPages(Pageframe& page);

    u64 allocVirtMemory4KPages(u64 amount);
    void freeVirtMemory4KPages(u64 address);

    u64 alloc(u64 size);
    void free(u64 address);

    PageSection* sectionAt(u64 address);
    Pageframe* pfn2page(u64 pfn);
    u64 page2pfn(Pageframe& page);

    MemoryModelType getModelType() { return this->mmType; }
    PageAlloc* getPageAllocator() { return this->pageAlloc; }
    MemoryAlloc* getKernelAllocator() { return this->memoryAlloc; }

    u64 getTotalPages() { return this->totalPages; }
    u64 getAvailablePages() { return this->availablePages; }
    u64 getAllocatedPages() { return this->allocatedPages; }
    u64 getCachedPages() { return this->cachedPages; }
    u64 getSwappedPages() { return this->swappedPages; }
    void calculate();

    PageSection** sections;

private:
    u64 totalPages, availablePages, allocatedPages, cachedPages, swappedPages;
    MemoryModelType mmType;
    PageAlloc* pageAlloc;
    MemoryAlloc* memoryAlloc;
};