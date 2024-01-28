#include <siberix/mem/address-space.h>
#include <siberix/mem/page.h>
#include <stdcxx/array.h>
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
            uint64_t _present : 1;
            uint64_t _writable : 1;
            uint64_t _user : 1;
            uint64_t _writeThrough : 1;
            uint64_t _cacheDisabled : 1;
            uint64_t _accessed : 1;
            uint64_t _ignored : 1;
            uint64_t _pageSize : 1;
            uint64_t _ignored2 : 4;
            uint64_t _address : 40;
            uint64_t _ignored3 : 11;
            uint64_t _disableExecute : 1;
        };
        uint64_t _raw;

        constexpr Pml4Entry& present(bool present = true)
        {
            _present = present;
            return *this;
        }
        constexpr Pml4Entry& writable(bool writable = true)
        {
            _writable = writable;
            return *this;
        }
        constexpr Pml4Entry& user(bool user = true)
        {
            _user = user;
            return *this;
        }
        constexpr Pml4Entry& writeThrough(bool writeThrough = true)
        {
            _writeThrough = writeThrough;
            return *this;
        }
        constexpr Pml4Entry& cacheDisabled(bool cacheDisabled = true)
        {
            _cacheDisabled = cacheDisabled;
            return *this;
        }
        constexpr Pml4Entry& accessed(bool accessed = true)
        {
            _accessed = accessed;
            return *this;
        }
        constexpr Pml4Entry& pageSize(bool pageSize = true)
        {
            _pageSize = pageSize;
            return *this;
        }
        constexpr Pml4Entry& address(uint64_t address)
        {
            _address = address & PAGE_FRAME;
            return *this;
        }
        constexpr Pml4Entry& disableExecute(bool disableExecute = true)
        {
            _disableExecute = disableExecute;
            return *this;
        }
    } __attribute__((packed));

    union PdptEntry
    {
        struct
        {
            uint64_t _present : 1;
            uint64_t _writable : 1;
            uint64_t _user : 1;
            uint64_t _writeThrough : 1;
            uint64_t _cacheDisabled : 1;
            uint64_t _accessed : 1;
            uint64_t _ignored : 1;
            uint64_t _pageSize : 1;
            uint64_t _ignored2 : 4;
            uint64_t _address : 40;
            uint64_t _ignored3 : 11;
            uint64_t _disableExecute : 1;
        };
        uint64_t _raw;

        constexpr PdptEntry& present(bool present = true)
        {
            _present = present;
            return *this;
        }
        constexpr PdptEntry& writable(bool writable = true)
        {
            _writable = writable;
            return *this;
        }
        constexpr PdptEntry& user(bool user = true)
        {
            _user = user;
            return *this;
        }
        constexpr PdptEntry& writeThrough(bool writeThrough = true)
        {
            _writeThrough = writeThrough;
            return *this;
        }
        constexpr PdptEntry& cacheDisabled(bool cacheDisabled = true)
        {
            _cacheDisabled = cacheDisabled;
            return *this;
        }
        constexpr PdptEntry& accessed(bool accessed = true)
        {
            _accessed = accessed;
            return *this;
        }
        constexpr PdptEntry& pageSize(bool pageSize = true)
        {
            _pageSize = pageSize;
            return *this;
        }
        constexpr PdptEntry& address(uint64_t address)
        {
            _address = address & PAGE_FRAME;
            return *this;
        }
        constexpr PdptEntry& disableExecute(bool disableExecute = true)
        {
            _disableExecute = disableExecute;
            return *this;
        }

    } __attribute__((packed));

    union PdirEntry
    {
        struct
        {
            uint64_t _present : 1;
            uint64_t _writable : 1;
            uint64_t _user : 1;
            uint64_t _writeThrough : 1;
            uint64_t _cacheDisabled : 1;
            uint64_t _accessed : 1;
            uint64_t _dirty : 1;
            uint64_t _pageSize : 1;
            uint64_t _global : 1;
            uint64_t _ignored : 3;
            uint64_t _address : 40;
            uint64_t _ignored2 : 11;
            uint64_t _disableExecute : 1;
        };
        uint64_t _raw;

        constexpr PdirEntry& present(bool present = true)
        {
            _present = present;
            return *this;
        }
        constexpr PdirEntry& writable(bool writable = true)
        {
            _writable = writable;
            return *this;
        }
        constexpr PdirEntry& user(bool user = true)
        {
            _user = user;
            return *this;
        }
        constexpr PdirEntry& writeThrough(bool writeThrough = true)
        {
            _writeThrough = writeThrough;
            return *this;
        }
        constexpr PdirEntry& cacheDisabled(bool cacheDisabled = true)
        {
            _cacheDisabled = cacheDisabled;
            return *this;
        }
        constexpr PdirEntry& accessed(bool accessed = true)
        {
            _accessed = accessed;
            return *this;
        }
        constexpr PdirEntry& dirty(bool dirty = true)
        {
            _dirty = dirty;
            return *this;
        }
        constexpr PdirEntry& pageSize(bool pageSize = true)
        {
            _pageSize = pageSize;
            return *this;
        }
        constexpr PdirEntry& global(bool global = true)
        {
            _global = global;
            return *this;
        }
        constexpr PdirEntry& address(uint64_t address)
        {
            _address = address & PAGE_FRAME;
            return *this;
        }
        constexpr PdirEntry& disableExecute(bool disableExecute = true)
        {
            _disableExecute = disableExecute;
            return *this;
        }

    } __attribute__((packed));

    union PageEntry
    {
        struct
        {
            uint64_t _present : 1;
            uint64_t _writable : 1;
            uint64_t _user : 1;
            uint64_t _writeThrough : 1;
            uint64_t _cacheDisabled : 1;
            uint64_t _accessed : 1;
            uint64_t _dirty : 1;
            uint64_t _attribute : 1;
            uint64_t _global : 1;
            uint64_t _ignored : 3;
            uint64_t _address : 40;
            uint64_t _ignored2 : 11;
            uint64_t _disableExecute : 1;
        };
        uint64_t _raw;

        constexpr PageEntry& present(bool present = true)
        {
            _present = present;
            return *this;
        }
        constexpr PageEntry& writable(bool writable = true)
        {
            _writable = writable;
            return *this;
        }
        constexpr PageEntry& user(bool user = true)
        {
            _user = user;
            return *this;
        }
        constexpr PageEntry& writeThrough(bool writeThrough = true)
        {
            _writeThrough = writeThrough;
            return *this;
        }
        constexpr PageEntry& cacheDisabled(bool cacheDisabled = true)
        {
            _cacheDisabled = cacheDisabled;
            return *this;
        }
        constexpr PageEntry& accessed(bool accessed = true)
        {
            _accessed = accessed;
            return *this;
        }
        constexpr PageEntry& dirty(bool dirty = true)
        {
            _dirty = dirty;
            return *this;
        }
        constexpr PageEntry& attribute(bool attribute = true)
        {
            _attribute = attribute;
            return *this;
        }
        constexpr PageEntry& global(bool global = true)
        {
            _global = global;
            return *this;
        }
        constexpr PageEntry& address(uint64_t address)
        {
            _address = address & PAGE_FRAME;
            return *this;
        }
        constexpr PageEntry& disableExecute(bool disableExecute = true)
        {
            _disableExecute = disableExecute;
            return *this;
        }

    } __attribute__((packed));

    enum VmmFlags : uint64_t
    {
        Present        = 1 << 0,
        Writable       = 1 << 1,
        User           = 1 << 2,
        WriteThrough   = 1 << 3,
        DisableCache   = 1 << 4,
        Accessed       = 1 << 5,
        Dirty          = 1 << 6,
        Attribute      = 1 << 7,
        PageSize       = 1 << 7,
        Global         = 1 << 8,
        DisableExecute = 1 << 63,
    };

    template <uint8_t L>
    struct MapLevel
    {
        constexpr static size_t _lv  = L;
        constexpr static size_t _len = 512;

        union Entry
        {
            uint64_t _flags;
            /*
                |-----------------------------------------------------|
                | 8   | 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
    Universal   |     |     |     | ACC | CAC | WT  | USR | RW  | PRS |
    L1          | GLB | ATR | DRT |     |     |     |     |     |     |
    L2          | GLB | PS  | DRT |     |     |     |     |     |     |
    L3          |     | PS  |     |     |     |     |     |     |     |
    L4          |     | PS  |     |     |     |     |     |     |     |
                |-----------------------------------------------------|
             */
            struct
            {
                uint64_t _present : 1;
                uint64_t _writable : 1;
                /* Can this page be accessed by user-space program */
                uint64_t _user : 1;
                /*
                    In write-through caching, writes go to both the cache and
                    main memory, but reads come only from the cache. This
                    reduces the number of writes to main memory, but if the
                    cache is not kept coherent with main memory, stale data may
                    be read.
                 */
                uint64_t _writeThrough : 1;
                uint64_t _cacheDisabled : 1;
                uint64_t _accessed : 1;
                uint64_t _dirty : 1;
                uint64_t _pageSize : 1; // 'attribute' on page level 1 entry
                uint64_t _global : 1;
                uint64_t _ignored : 3;
                uint64_t _address : 40;
                uint64_t _ignored2 : 11;
                uint64_t _disableExecute : 1;
            };

            constexpr Entry& present(bool present = true)
            {
                _present = present;
                return *this;
            }
            constexpr Entry& writable(bool writable = true)
            {
                _writable = writable;
                return *this;
            }
            constexpr Entry& user(bool user = true)
            {
                _user = user;
                return *this;
            }
            constexpr Entry& writeThrough(bool writeThrough = true)
            {
                _writeThrough = writeThrough;
                return *this;
            }
            constexpr Entry& cacheDisabled(bool cacheDisabled = true)
            {
                _cacheDisabled = cacheDisabled;
                return *this;
            }
            constexpr Entry& accessed(bool accessed = true)
            {
                _accessed = accessed;
                return *this;
            }
            constexpr Entry& dirty(bool dirty = true)
            {
                _dirty = dirty;
                return *this;
            }
            constexpr Entry& pageSize(bool pageSize = true)
            {
                static_assert(L != 1);
                _pageSize = pageSize;
                return *this;
            }
            constexpr Entry& attribute(bool attribute = true)
            {
                static_assert(L == 1);
                _attribute = attribute;
                return *this;
            }
            constexpr Entry& global(bool global = true)
            {
                static_assert(L <= 2);
                _global = global;
                return *this;
            }
            constexpr Entry& address(uint64_t address)
            {
                _address = address & PAGE_FRAME;
                return *this;
            }
            constexpr Entry& disableExecute(bool disableExecute = true)
            {
                _disableExecute = disableExecute;
                return *this;
            }
            constexpr Entry& setFlags(uint64_t flags)
            {
                _flags |= flags;
                return *this;
            }
            constexpr Entry& clearFlags(uint64_t flags)
            {
                _flags &= ~flags;
                return *this;
            }
        } __attribute__((packed)) ents[_len];
        static_assert(sizeof(MapLevel<L>::Entry) == 8);

        // Entry ents[_len];

        Entry& operator[](uint64_t index) { return ents[index]; }

        Entry const& operator[](uint64_t index) const { return ents[index]; }

    } __attribute__((aligned(PAGE_SIZE_4K)));

    using Pml4 = MapLevel<4>; // 256 terabytes
    using Pml3 = MapLevel<3>; // 512 gigabytes, Page directory pointer table
    using Pml2 = MapLevel<2>; // 1 gigabyte, Page directory table
    using Pml1 = MapLevel<1>; // 2 megabytes, Page table
    static_assert(sizeof(Pml1) == PAGE_SIZE_4K);

    class X64Pages : public Mem::AddressSpace
    {
    public:
        X64Pages();
        X64Pages(MapLevel<3>* pdptOfKernel);
        ~X64Pages();

        uint64_t alloc4KPages(uint64_t amount,
                              bool     isWritable      = true,
                              bool     isWriteThrough  = false,
                              bool     isCacheDisabled = false) override;
        void     free4KPages(uint64_t address, uint64_t amount) override;
        void map4KPages(uint64_t phys, uint64_t virt, uint64_t amount) override;
        bool isPagePresent(uint64_t address) override;
        uint64_t convertVirtToPhys(uint64_t address) override;

        void load() override
        {
            asm volatile("mov %%rax, %%cr3" : : "a"(_pml4Phys));
        }

        MapLevel<4>               _pml4;
        MapLevel<3>               _pdpt;
        /*
            Theoretically, a program can take up to 512 gigabytes space.
            This is exactly the size of a page directory pointer table, aka
            PDPT, which is 4096 bytes * 512 entries (a page table) * 512 entries
            (a page directory) * 512 entries (a page directory pointer table) =
            512 GB.

            This _pageDirs pointer will actually point to an array of pointers,
            each of which points to a page directory table.
         */
        Array<MapLevel<2>*, 512>  _pageDirs;
        /*
            _pageTables is a two-dimensional array of pointers, each of which
            points to a page table. The first dimension is the index of the
            page directory pointer table, and the second dimension is the index
            of the page directory table.
         */
        Array<MapLevel<1>**, 512> _pageTables;

        uint64_t               _pml4Phys;
        MapLevel<3>*           _pdptOfKern;
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
            equals to "uint64_t array[16384 / 8][1024]"
         */
        Std::Bitmap<uint64_t>* _bitmap;
    };

    class X64KernelPages : public X64Pages
    {
    public:
        X64KernelPages();

        uint64_t alloc4KPages(uint64_t amount,
                              bool     isWritable      = true,
                              bool     isWriteThrough  = false,
                              bool     isCacheDisabled = false) override;
        void     free4KPages(uint64_t address, uint64_t amount) override;
        void map4KPages(uint64_t phys, uint64_t virt, uint64_t amount) override;
        bool isPagePresent(uint64_t address) override;
        uint64_t convertVirtToPhys(uint64_t address) override;
    };

    inline constexpr uint16_t pml4IndexOf(uint64_t addr)
    {
        return (addr >> 39) & 0x1FF;
    }

    inline constexpr uint16_t pdptIndexOf(uint64_t addr)
    {
        return (addr >> 30) & 0x1FF;
    }

    inline constexpr uint16_t pdirIndexOf(uint64_t addr)
    {
        return (addr >> 21) & 0x1FF;
    }

    inline constexpr uint16_t pageIndexOf(uint64_t addr)
    {
        return (addr >> 12) & 0x1FF;
    }
}