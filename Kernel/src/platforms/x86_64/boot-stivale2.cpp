#include <boot-protocols/stivale2.h>
#include <siberix/kmain.h>

using namespace Kern::Init;

extern "C" [[noreturn]] void
kload_st2(stivale2_struct* stInfo)
{
    if (!stInfo) {
        // TODO: Kernel panic
    }
    static BootConfigTable bootConfig;

    stivale2_tag* stTags = reinterpret_cast<stivale2_tag*>(stInfo->tags);
    while (stTags) {
        switch (stTags->identifier) {
            case STIVALE2_STRUCT_TAG_CMDLINE_ID: {
            }
            case STIVALE2_STRUCT_TAG_MEMMAP_ID: {
            }
        }
    }
    kmain(bootConfig);
}