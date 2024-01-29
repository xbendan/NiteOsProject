#include <arch-x86_64/hwmem.h>
#include <siberix/mem/mem.h>
#include <siberix/mem/range.h>

namespace Kern::Mem {
    uint64_t copyAsIoAddress(uint64_t address)
    {
        return IO_VIRTUAL_BASE + address;
    }
}

namespace Kern::Platform::X64 {
    using namespace Kern::Mem;

    MapLevel<2> kPageDirs;
    MapLevel<2> kHeapDirs;
    MapLevel<2> kIoDirs[4];
    MapLevel<1> kHeapTables[TABLES_PER_DIR];
    uint64_t    kHeapBitmap[0x1000];

    static MapLevel<1>* kPageTablePointers[DIRS_PER_PDPT][TABLES_PER_DIR];

    X64Pages::X64Pages(MapLevel<3>* pdptOfKern)
      : _pml4Phys((uint64_t)&_pml4 - KERNEL_VIRTUAL_BASE)
      , _pdptOfKern(pdptOfKern)
      , _bitmap(new Std::BitmapDouble<uint64_t>(16384 / 8, 1024))
    {
        _pml4[0] //
          .setFlags(VmmFlags::Present | VmmFlags::Writable | VmmFlags::User)
          .address((uint64_t)&_pdpt - KERNEL_VIRTUAL_BASE);
        _pdpt[0] //
          .setFlags(VmmFlags::Present | VmmFlags::Writable | VmmFlags::User)
          .address((uint64_t)&_pageDirs[0] - KERNEL_VIRTUAL_BASE);

        /*
            Pre-allocate 128 pages (0.5 MB) for loading executable files.
         */
        alloc4KPages(128);
    }

    X64Pages::~X64Pages()
    {
        for (int i = 0; i < MapLevel<2>::_len; i++) {
            if (!_pageDirs[i]) {
                break;
            }
            delete _pageDirs[i];
            if (!_pageTables[i]) {
                continue;
            }
            for (int j = 0; j < MapLevel<1>::_len; j++) {
                if (!_pageTables[i][j]) {
                    break;
                }
                delete _pageTables[i][j];
            }
        }
        delete _bitmap;
        // Do we delete all the page tables?
        // I hate nested pointers.
    }

    uint64_t X64Pages::alloc4KPages(uint64_t amount,
                                    bool     rw,
                                    bool     pwt,
                                    bool     pcd)
    {
        uint64_t bitmapIndex = _bitmap->findFree(amount);
        uint16_t i, j, k;
        while (amount) {
            i = (bitmapIndex >> 18) & 0x1ff;
            j = (bitmapIndex >> 9) & 0x1ff;
            k = bitmapIndex & 0x1ff;
            if (!_pageDirs[i]) {
                _pageDirs[i]   = new MapLevel<2>();
                _pageTables[i] = new MapLevel<1>*[DIRS_PER_PDPT];
                _pdpt[i] //
                  .present()
                  .writable(rw)
                  .user()
                  .writeThrough(pwt)
                  .cacheDisabled(pcd)
                  .address((uint64_t)&_pageDirs[i] - KERNEL_VIRTUAL_BASE);
            }

            if (!_pageTables[i][j]) {
                _pageTables[i][j] = new MapLevel<1>();
                (*_pageDirs[i])[j] //
                  .present()
                  .writable(rw)
                  .user()
                  .writeThrough(pwt)
                  .cacheDisabled(pcd)
                  .address((uint64_t)&_pageTables[i][j] - KERNEL_VIRTUAL_BASE);
            }

            (*_pageTables[i][j])[k] //
              .present()
              .writable(rw)
              .user()
              .writeThrough(pwt)
              .cacheDisabled(pcd)
              .address(m_zeroPage);

            amount--;
            bitmapIndex++;
        }
        return bitmapIndex << 12;
    }

    void X64Pages::free4KPages(uint64_t address, uint64_t amount) {}

    void X64Pages::map4KPages(uint64_t phys, uint64_t virt, uint64_t amount) {}

    bool X64Pages::isPagePresent(uint64_t address)
    {
        // TODO: Implement
        return false;
    }

    uint64_t X64Pages::convertVirtToPhys(uint64_t address)
    {
        // TODO: Implement
        return 0;
    }

    X64KernelPages::X64KernelPages()
    {
        rangeOf(&_pml4, sizeof(MapLevel<4>)).clear();
        rangeOf(&_pdpt, sizeof(MapLevel<3>)).clear();

        _pml4[pml4IndexOf(KERNEL_VIRTUAL_BASE)] //
          .present()
          .writable()
          .address((uint64_t)&_pdpt - KERNEL_VIRTUAL_BASE);
        _pml4[0] = _pml4[pml4IndexOf(KERNEL_VIRTUAL_BASE)];

        _pdpt[pdptIndexOf(KERNEL_VIRTUAL_BASE)].present().writable().address(
          (uint64_t)&kPageDirs - KERNEL_VIRTUAL_BASE);
        _pageDirs[0] = &kPageDirs;

        for (int i = 0; i < TABLES_PER_DIR; i++) {
            kPageDirs[i]
              .present()
              .writable()
              .cacheDisabled()
              .pageSize()
              .address((uint64_t)&kHeapTables[i] - KERNEL_VIRTUAL_BASE);
        }

        // Set kernel heap tables
        _pdpt[DIRS_PER_PDPT - 1] //
          .present()
          .writable()
          .address((uint64_t)&kHeapDirs - KERNEL_VIRTUAL_BASE);
        for (int i = 0; i < TABLES_PER_DIR; i++) {
            kHeapDirs[i] //
              .present()
              .writable()
              .address((uint64_t)&kHeapTables[i] - KERNEL_VIRTUAL_BASE);
            kPageTablePointers[DIRS_PER_PDPT - 1][i] = &kHeapTables[i];
        }

        // Set IO tables
        for (int i = 0; i < 4; i++) {
            _pdpt[pdptIndexOf(IO_VIRTUAL_BASE) + i]
              .present()
              .writable()
              .address((uint64_t)&kIoDirs[i] - KERNEL_VIRTUAL_BASE);
            for (int j = 0; j < TABLES_PER_DIR; j++) {
                kIoDirs[i][j] //
                  .present()
                  .writable()
                  .cacheDisabled()
                  .pageSize()
                  .address((uint64_t)(PAGE_SIZE_1G * i + PAGE_SIZE_2M * j));
            }
        }
        _pdpt[0]  = _pdpt[pdptIndexOf(KERNEL_VIRTUAL_BASE)];
        _pml4Phys = (uint64_t)&_pml4 - KERNEL_VIRTUAL_BASE;
    }

    uint64_t X64KernelPages::alloc4KPages(uint64_t amount,
                                          bool     isWritable,
                                          bool     isWriteThrough,
                                          bool     isCacheDisabled)
    {
        return 0;
    }

    void X64KernelPages::free4KPages(uint64_t address, uint64_t amount) {}

    void X64KernelPages::map4KPages(uint64_t phys,
                                    uint64_t virt,
                                    uint64_t amount)
    {
    }

    bool X64KernelPages::isPagePresent(uint64_t address)
    {
        return false;
    }

    uint64_t X64KernelPages::convertVirtToPhys(uint64_t address)
    {
        return 0;
    }
}