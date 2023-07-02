#include <common/typedefs.h>

#ifdef ARCH_X86_64
#define BUILD_ARCH "x86_64"
#else ARCH_RISCV
#define BUILD_ARCH "riscv"
#endif

enum BootloaderName
{
    BL_LIMINE,
    BL_NBW
};

struct BootConfig {
    u64 checksum;
    BootloaderName blName;
    struct {
        u16 width;
        u16 height;
        u64 address;
        u16 pitch;
        u16 bpp;
    } graphic;
}

void
main(BootConfig &bootConfig);