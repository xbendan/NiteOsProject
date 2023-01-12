#define KERNEL_VIRTUAL_BASE     0xffffffff80000000
#define IO_VIRTUAL_BASE         KERNEL_VIRTUAL_BASE - 0x100000000

#define USER_SPACE              0x0000000000000000
#define KERNEL_SPACE            0xFFFFFF8000000000
#define KERNEL_PAGEMAPS         0xFFFFFF8000000000 // ~0xFFFFFF8FFFFFFFFF 64GiB

#define LOWMEM_RESERVED         0x100000

typedef uint64_t address_t;