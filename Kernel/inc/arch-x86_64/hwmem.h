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

    using Pml4      = Pml4Entry[PDPTS_PER_PML4];  // 256 terabytes
    using Pdpt      = PdptEntry[DIRS_PER_PDPT];   // 512 gigabytes
    using PdirTable = PdirEntry[TABLES_PER_DIR];  // 1 gigabyte
    using PageTable = PageEntry[PAGES_PER_TABLE]; // 2 megabytes

    class X64Pages : public Mem::AddressSpace
    {
    public:
        X64Pages();
        X64Pages(Pdpt* pdptOfKernel);
        ~X64Pages();

        uint64_t alloc4KPages(uint64_t amount,
                              bool     isWritable      = true,
                              bool     isWriteThrough  = false,
                              bool     isCacheDisabled = false,
                              bool     directMap2M     = true) override;
        void     free4KPages(uint64_t address, uint64_t amount) override;
        void     map(uint64_t phys, uint64_t virt, uint64_t amount) override;
        bool     isPagePresent(uint64_t address) override;
        uint64_t convertVirtToPhys(uint64_t address) override;

        void load() override
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

        uint64_t               _pml4Phys;
        Pdpt*                  _pdptOfKern;
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
                              bool     isCacheDisabled = false,
                              bool     directMap2M     = true) override;
        void     free4KPages(uint64_t address, uint64_t amount) override;
        void     map(uint64_t phys, uint64_t virt, uint64_t amount) override;
        bool     isPagePresent(uint64_t address) override;
        uint64_t convertVirtToPhys(uint64_t address) override;
    };

    inline constexpr uint8_t pml4IndexOf(uint64_t addr)
    {
        return (addr >> 39) & 0x1FF;
    }

    inline constexpr uint8_t pdptIndexOf(uint64_t addr)
    {
        return (addr >> 30) & 0x1FF;
    }

    inline constexpr uint8_t pdirIndexOf(uint64_t addr)
    {
        return (addr >> 21) & 0x1FF;
    }

    inline constexpr uint8_t pageIndexOf(uint64_t addr)
    {
        return (addr >> 12) & 0x1FF;
    }
}