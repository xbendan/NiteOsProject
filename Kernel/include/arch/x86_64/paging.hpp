#include <common/typedefs.h>
#include <siberix/mm/addrspace.hpp>
#include <siberix/mm/page.hpp>

#define PAGES_PER_TABLE             512
#define TABLES_PER_DIR              512
#define DIRS_PER_PDPT               512
#define PDPTS_PER_PML4              512
#define PML4_GET_INDEX(addr)        (((addr) >> 39) & 0x1FF)
#define PDPT_GET_INDEX(addr)        (((addr) >> 30) & 0x1FF)
#define PDIR_GET_INDEX(addr)        (((addr) >> 21) & 0x1FF)
#define PAGE_GET_INDEX(addr)        (((addr) >> 12) & 0x1FF)

namespace Paging
{
    typedef uint64_t pml4e_t;
    typedef uint64_t pdpte_t;
    typedef uint64_t pde_t;
    typedef uint64_t page_t;

    using pml4_t = pml4e_t[PDPTS_PER_PML4]; /* 512GiB -> 256TiB */
    using pdpt_t = pdpte_t[DIRS_PER_PDPT]; /* 1GiB -> 512GiB */
    using pagedir_t = pde_t[TABLES_PER_DIR]; /* 2MiB -> 1GiB */
    using pagetable_t = page_t[PAGES_PER_TABLE]; /* 4KiB -> 2MiB */

    enum PageFlags
    {
        PagePresent = 0x01,
        PageWritable = 0x02,
        PageUser = 0x04,
        PageWriteThrough = 0x08,
        PageCacheDisabled = 0x10,
        PageDirectAddress = 0x80
    };

    class X64AddressSpace : public AddressSpace
    {
    public:
        pml4_t pml4 __attribute__((aligned(PAGE_SIZE_4K)));
        pdpt_t pdpt __attribute__((aligned(PAGE_SIZE_4K)));
        pde_t *pageDirs[DIRS_PER_PDPT] __attribute__((aligned(PAGE_SIZE_4K)));
        page_t **pageTables[DIRS_PER_PDPT] __attribute__((aligned(PAGE_SIZE_4K)));
        u64 pml4Phys;
        pdpt_t *kernelPdpts;

        X64AddressSpace();
        ~X64AddressSpace();
    } __attribute__((packed));
}