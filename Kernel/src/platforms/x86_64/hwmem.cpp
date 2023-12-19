#include <arch-x86_64/hwmem.h>
#include <siberix/mem/mem-range.h>

namespace Kern::Platform::X64 {
    using namespace Kern::Mem;

    static inline PdirTable kPageDirs __attribute__((aligned(PAGE_SIZE_4K)));
    static inline PdirTable kHeapDirs __attribute__((aligned(PAGE_SIZE_4K)));
    static inline PdirTable kIoDirs[4] __attribute__((aligned(PAGE_SIZE_4K)));
    static inline PageTable kHeapTables[TABLES_PER_DIR]
      __attribute__((aligned(PAGE_SIZE_4K)));
    static inline PageTable* kPageTablePointers[DIRS_PER_PDPT][TABLES_PER_DIR];

    X64Pages::X64Pages()
      : _bitmap(new Std::BitmapDouble<UInt64>(16384 / 8, 1024))
      , _pml4Phys((UInt64)&_pml4 - KERNEL_VIRTUAL_BASE)
    {
        _pml4[0] //
          .present()
          .writable()
          .user()
          .address((UInt64)&_pdpt - KERNEL_VIRTUAL_BASE);
        _pdpt[0] //
          .present()
          .writable()
          .user()
          .address((UInt64)&_pageDirs[0] - KERNEL_VIRTUAL_BASE);
        _pageDirs   = new PdirTable*[DIRS_PER_PDPT];
        _pageTables = new PageTable**[DIRS_PER_PDPT];

        /*
            Pre-allocate 128 pages (0.5 MB) for loading executable files.
         */
        alloc4KPages(128);
    }

    X64Pages::~X64Pages()
    {
        delete _bitmap;
        for (UInt16 pageDirsOffset = 0; pageDirsOffset < DIRS_PER_PDPT &&
                                        pageDirsOffset * PAGE_SIZE_1G < m_bound;
             pageDirsOffset++) {
            delete _pageDirs[pageDirsOffset];
            if (!_pageTables[pageDirsOffset]) {
                continue;
            }
            for (UInt16 pageTableOffset = 0; pageTableOffset < TABLES_PER_DIR;
                 pageTableOffset++) {
                delete _pageTables[pageDirsOffset][pageTableOffset];
            }
        }
        // Do we delete all the page tables?
        // I hate nested pointers.
    }

    UInt64 X64Pages::alloc4KPages(UInt64 amount)
    {
        if (!_bitmap) {
            _bitmap =
              new Std::BitmapDouble<UInt64>(DIRS_PER_PDPT, TABLES_PER_DIR);
        }
        UInt64 bitmapIndex = _bitmap->findFree(amount);
        UInt64 pageIndex   = 0;
        UInt64 pdirIndex   = 0;
        UInt64 pdptIndex   = 0;
        while (amount) {
            pageIndex = bitmapIndex & 512;
            pdirIndex = bitmapIndex >> 9;
            pdptIndex = bitmapIndex >> 18;
        }
    }

    Void X64Pages::free4KPages(UInt64 address, UInt64 amount) {}

    Void X64Pages::map(UInt64 phys, UInt64 virt, UInt64 amount) {}

    Boolean X64Pages::isPagePresent(UInt64 address) {}

    UInt64 X64Pages::convertVirtToPhys(UInt64 address) {}

    X64KernelPages::X64KernelPages()
    {
        rangeOf(&_pml4, sizeof(Pml4)).clear();
        rangeOf(&_pdpt, sizeof(Pdpt)).clear();

        _pml4[pml4IndexOf(KERNEL_VIRTUAL_BASE)] //
          .present()
          .writable()
          .address((UInt64)&_pdpt - KERNEL_VIRTUAL_BASE);
        _pml4[0] = _pml4[pml4IndexOf(KERNEL_VIRTUAL_BASE)];

        _pdpt[pdptIndexOf(KERNEL_VIRTUAL_BASE)].present().writable().address(
          (UInt64)&kPageDirs - KERNEL_VIRTUAL_BASE);
        _pageDirs[0] = &kPageDirs;

        for (int i = 0; i < TABLES_PER_DIR; i++) {
            kPageDirs[i]
              .present()
              .writable()
              .cacheDisabled()
              .pageSize()
              .address((UInt64)&kHeapTables[i] - KERNEL_VIRTUAL_BASE);
        }

        // Set kernel heap tables
        _pdpt[DIRS_PER_PDPT - 1] //
          .present()
          .writable()
          .address((UInt64)&kHeapDirs - KERNEL_VIRTUAL_BASE);
        for (int i = 0; i < TABLES_PER_DIR; i++) {
            kHeapDirs[i] //
              .present()
              .writable()
              .address((UInt64)&kHeapTables[i] - KERNEL_VIRTUAL_BASE);
            kPageTablePointers[DIRS_PER_PDPT - 1][i] = &kHeapTables[i];
        }

        // Set IO tables
        for (int i = 0; i < 4; i++) {
            _pdpt[pdptIndexOf(IO_VIRTUAL_BASE) + i]
              .present()
              .writable()
              .address((UInt64)&kIoDirs[i] - KERNEL_VIRTUAL_BASE);
            for (int j = 0; j < TABLES_PER_DIR; j++) {
                kIoDirs[i][j] //
                  .present()
                  .writable()
                  .cacheDisabled()
                  .pageSize()
                  .address((UInt64)(PAGE_SIZE_1G * i + PAGE_SIZE_2M * j));
            }
        }
        _pdpt[0]  = _pdpt[pdptIndexOf(KERNEL_VIRTUAL_BASE)];
        _pml4Phys = (UInt64)&_pml4 - KERNEL_VIRTUAL_BASE;
    }

    UInt64 X64KernelPages::alloc4KPages(UInt64 amount) {}

}