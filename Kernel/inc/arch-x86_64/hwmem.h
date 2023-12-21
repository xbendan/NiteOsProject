#include <siberix/mem/address-space.h>
#include <siberix/mem/page.h>
#include <stdcxx/bitmap.h>

#define PAGES_PER_TABLE 512
#define TABLES_PER_DIR 512
#define DIRS_PER_PDPT 512
#define PDPTS_PER_PML4 512
#define PAGE_FRAME 0xFFFFFFFFF000ULL

#define KERNEL_VIRTUAL_BASE 0xffffffff80000000
#define IO_VIRTUAL_BASE (KERNEL_VIRTUAL_BASE - 0x100000000)

namespace Kern::Platform::X64 {
    union Pml4Entry
    {
        struct
        {
            UInt64 _present : 1;
            UInt64 _writable : 1;
            UInt64 _user : 1;
            UInt64 _writeThrough : 1;
            UInt64 _cacheDisabled : 1;
            UInt64 _accessed : 1;
            UInt64 _ignored : 1;
            UInt64 _pageSize : 1;
            UInt64 _ignored2 : 4;
            UInt64 _address : 40;
            UInt64 _ignored3 : 11;
            UInt64 _disableExecute : 1;
        };
        UInt64 _raw;

        constexpr Pml4Entry& present(Boolean present = true)
        {
            _present = present;
            return *this;
        }
        constexpr Pml4Entry& writable(Boolean writable = true)
        {
            _writable = writable;
            return *this;
        }
        constexpr Pml4Entry& user(Boolean user = true)
        {
            _user = user;
            return *this;
        }
        constexpr Pml4Entry& writeThrough(Boolean writeThrough = true)
        {
            _writeThrough = writeThrough;
            return *this;
        }
        constexpr Pml4Entry& cacheDisabled(Boolean cacheDisabled = true)
        {
            _cacheDisabled = cacheDisabled;
            return *this;
        }
        constexpr Pml4Entry& accessed(Boolean accessed = true)
        {
            _accessed = accessed;
            return *this;
        }
        constexpr Pml4Entry& pageSize(Boolean pageSize = true)
        {
            _pageSize = pageSize;
            return *this;
        }
        constexpr Pml4Entry& address(UInt64 address)
        {
            _address = address & PAGE_FRAME;
            return *this;
        }
        constexpr Pml4Entry& disableExecute(Boolean disableExecute = true)
        {
            _disableExecute = disableExecute;
            return *this;
        }
    } __attribute__((packed));

    union PdptEntry
    {
        struct
        {
            UInt64 _present : 1;
            UInt64 _writable : 1;
            UInt64 _user : 1;
            UInt64 _writeThrough : 1;
            UInt64 _cacheDisabled : 1;
            UInt64 _accessed : 1;
            UInt64 _ignored : 1;
            UInt64 _pageSize : 1;
            UInt64 _ignored2 : 4;
            UInt64 _address : 40;
            UInt64 _ignored3 : 11;
            UInt64 _disableExecute : 1;
        };
        UInt64 _raw;

        constexpr PdptEntry& present(Boolean present = true)
        {
            _present = present;
            return *this;
        }
        constexpr PdptEntry& writable(Boolean writable = true)
        {
            _writable = writable;
            return *this;
        }
        constexpr PdptEntry& user(Boolean user = true)
        {
            _user = user;
            return *this;
        }
        constexpr PdptEntry& writeThrough(Boolean writeThrough = true)
        {
            _writeThrough = writeThrough;
            return *this;
        }
        constexpr PdptEntry& cacheDisabled(Boolean cacheDisabled = true)
        {
            _cacheDisabled = cacheDisabled;
            return *this;
        }
        constexpr PdptEntry& accessed(Boolean accessed = true)
        {
            _accessed = accessed;
            return *this;
        }
        constexpr PdptEntry& pageSize(Boolean pageSize = true)
        {
            _pageSize = pageSize;
            return *this;
        }
        constexpr PdptEntry& address(UInt64 address)
        {
            _address = address & PAGE_FRAME;
            return *this;
        }
        constexpr PdptEntry& disableExecute(Boolean disableExecute = true)
        {
            _disableExecute = disableExecute;
            return *this;
        }

    } __attribute__((packed));

    union PdirEntry
    {
        struct
        {
            UInt64 _present : 1;
            UInt64 _writable : 1;
            UInt64 _user : 1;
            UInt64 _writeThrough : 1;
            UInt64 _cacheDisabled : 1;
            UInt64 _accessed : 1;
            UInt64 _dirty : 1;
            UInt64 _pageSize : 1;
            UInt64 _global : 1;
            UInt64 _ignored : 3;
            UInt64 _address : 40;
            UInt64 _ignored2 : 11;
            UInt64 _disableExecute : 1;
        };
        UInt64 _raw;

        constexpr PdirEntry& present(Boolean present = true)
        {
            _present = present;
            return *this;
        }
        constexpr PdirEntry& writable(Boolean writable = true)
        {
            _writable = writable;
            return *this;
        }
        constexpr PdirEntry& user(Boolean user = true)
        {
            _user = user;
            return *this;
        }
        constexpr PdirEntry& writeThrough(Boolean writeThrough = true)
        {
            _writeThrough = writeThrough;
            return *this;
        }
        constexpr PdirEntry& cacheDisabled(Boolean cacheDisabled = true)
        {
            _cacheDisabled = cacheDisabled;
            return *this;
        }
        constexpr PdirEntry& accessed(Boolean accessed = true)
        {
            _accessed = accessed;
            return *this;
        }
        constexpr PdirEntry& dirty(Boolean dirty = true)
        {
            _dirty = dirty;
            return *this;
        }
        constexpr PdirEntry& pageSize(Boolean pageSize = true)
        {
            _pageSize = pageSize;
            return *this;
        }
        constexpr PdirEntry& global(Boolean global = true)
        {
            _global = global;
            return *this;
        }
        constexpr PdirEntry& address(UInt64 address)
        {
            _address = address & PAGE_FRAME;
            return *this;
        }
        constexpr PdirEntry& disableExecute(Boolean disableExecute = true)
        {
            _disableExecute = disableExecute;
            return *this;
        }

    } __attribute__((packed));

    union PageEntry
    {
        struct
        {
            UInt64 _present : 1;
            UInt64 _writable : 1;
            UInt64 _user : 1;
            UInt64 _writeThrough : 1;
            UInt64 _cacheDisabled : 1;
            UInt64 _accessed : 1;
            UInt64 _dirty : 1;
            UInt64 _attribute : 1;
            UInt64 _global : 1;
            UInt64 _ignored : 3;
            UInt64 _address : 40;
            UInt64 _ignored2 : 11;
            UInt64 _disableExecute : 1;
        };
        UInt64 _raw;

        constexpr PageEntry& present(Boolean present = true)
        {
            _present = present;
            return *this;
        }
        constexpr PageEntry& writable(Boolean writable = true)
        {
            _writable = writable;
            return *this;
        }
        constexpr PageEntry& user(Boolean user = true)
        {
            _user = user;
            return *this;
        }
        constexpr PageEntry& writeThrough(Boolean writeThrough = true)
        {
            _writeThrough = writeThrough;
            return *this;
        }
        constexpr PageEntry& cacheDisabled(Boolean cacheDisabled = true)
        {
            _cacheDisabled = cacheDisabled;
            return *this;
        }
        constexpr PageEntry& accessed(Boolean accessed = true)
        {
            _accessed = accessed;
            return *this;
        }
        constexpr PageEntry& dirty(Boolean dirty = true)
        {
            _dirty = dirty;
            return *this;
        }
        constexpr PageEntry& attribute(Boolean attribute = true)
        {
            _attribute = attribute;
            return *this;
        }
        constexpr PageEntry& global(Boolean global = true)
        {
            _global = global;
            return *this;
        }
        constexpr PageEntry& address(UInt64 address)
        {
            _address = address & PAGE_FRAME;
            return *this;
        }
        constexpr PageEntry& disableExecute(Boolean disableExecute = true)
        {
            _disableExecute = disableExecute;
            return *this;
        }

    } __attribute__((packed));

    using Pml4      = Pml4Entry[PDPTS_PER_PML4];  // 256 terabytes
    using Pdpt      = PdptEntry[DIRS_PER_PDPT];   // 512 gigabytes
    using PdirTable = PdirEntry[TABLES_PER_DIR];  // 1 gigabyte
    using PageTable = PageEntry[PAGES_PER_TABLE]; // 2 megabytes

    class X64Pages : public Mem::AddressSpace
    {
    public:
        X64Pages() = delete;
        ~X64Pages();

        UInt64  alloc4KPages(UInt64  amount,
                             Boolean isWritable      = true,
                             Boolean isWriteThrough  = false,
                             Boolean isCacheDisabled = false,
                             Boolean directMap2M     = true) override;
        Void    free4KPages(UInt64 address, UInt64 amount) override;
        Void    map(UInt64 phys, UInt64 virt, UInt64 amount) override;
        Boolean isPagePresent(UInt64 address) override;
        UInt64  convertVirtToPhys(UInt64 address) override;

        Void load() override
        {
            asm volatile("mov %%rax, %%cr3" : : "a"(_pml4Phys));
        }

        Pml4         _pml4 __attribute__((aligned(PAGE_SIZE_4K)));
        Pdpt         _pdpt __attribute__((aligned(PAGE_SIZE_4K)));
        /*
            Theoretically, a program can take up to 512 gigabytes space.
            This is exactly the size of a page directory pointer table, aka
            PDPT, which is 4096 bytes * 512 entries (a page table) * 512 entries
            (a page directory) * 512 entries (a page directory pointer table) =
            512 GB.

            This _pageDirs pointer will actually point to an array of pointers,
            each of which points to a page directory table.
         */
        PdirTable**  _pageDirs;
        /*
            _pageTables is a two-dimensional array of pointers, each of which
            points to a page table. The first dimension is the index of the
            page directory pointer table, and the second dimension is the index
            of the page directory table.
         */
        PageTable*** _pageTables;

        UInt64               _pml4Phys;
        Pdpt*                _pdptOfKernel;
        /*
            For each user-mode address space, the bitmap is used to track the
            allocated pages. In total, each address space can take up to 512
            gigabytes space, which is 512 * 1024 * 1024 * 1024 / 4096 =
            134,217,728 pages. This means that the bitmap could also take up to
            134,217,728 / 8 = 16,777,216 bytes = 16 megabytes. This is a huge
            amount of memory, consider that not all of the pages are used.
            Therefore, we use a two-dimensional array to track it. Each time we
            allocate 4 pages (16384 bytes) for the record, these pages can track
            about 16384 * 8 = 131,072 pages, hence the 2D array is exactly
            equals to "UInt64 array[16384 / 8][1024]"
         */
        Std::Bitmap<UInt64>* _bitmap;
    };

    class X64KernelPages : public X64Pages
    {
    public:
        X64KernelPages();

        UInt64  alloc4KPages(UInt64  amount,
                             Boolean isWritable      = true,
                             Boolean isWriteThrough  = false,
                             Boolean isCacheDisabled = false,
                             Boolean directMap2M     = true) override;
        Void    free4KPages(UInt64 address, UInt64 amount) override;
        Void    map(UInt64 phys, UInt64 virt, UInt64 amount) override;
        Boolean isPagePresent(UInt64 address) override;
        UInt64  convertVirtToPhys(UInt64 address) override;
    };

    inline constexpr UInt8 pml4IndexOf(UInt64 addr)
    {
        return (addr >> 39) & 0x1FF;
    }

    inline constexpr UInt8 pdptIndexOf(UInt64 addr)
    {
        return (addr >> 30) & 0x1FF;
    }

    inline constexpr UInt8 pdirIndexOf(UInt64 addr)
    {
        return (addr >> 21) & 0x1FF;
    }

    inline constexpr UInt8 pageIndexOf(UInt64 addr)
    {
        return (addr >> 12) & 0x1FF;
    }
}