#include <boot-protocols/stivale2.h>
#include <siberix/main.h>

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
                break;
            }
            case STIVALE2_STRUCT_TAG_MEMMAP_ID: {
                stivale2_struct_tag_memmap* tagMemmap =
                  reinterpret_cast<stivale2_struct_tag_memmap*>(stTags);
                for (uint64_t i = 0; i < tagMemmap->entries; i++) {
                    stivale2_mmap_entry* entry = &tagMemmap->memmap[i];
                    uint64_t             start = entry->base;
                    uint64_t             end   = entry->base + entry->length;

                    bootConfig._mem._ranges[i] = BootConfigTable::Mem::Memmap(
                      start, end, static_cast<uint8_t>(entry->type));
                    bootConfig._mem._totalSize += entry->length;
                    switch (entry->type) {
                        case STIVALE2_MMAP_USABLE:
                            bootConfig._mem._usableSize += entry->length;
                            bootConfig._mem._ranges->_type = BOOT_MEMMAP_USABLE;
                            break;
                        case STIVALE2_MMAP_KERNEL_AND_MODULES:
                            bootConfig._mem._ranges->_type =
                              BOOT_MEMMAP_KERNEL_AND_MODULES;
                            break;
                        case STIVALE2_MMAP_ACPI_RECLAIMABLE:
                            bootConfig._mem._ranges->_type =
                              BOOT_MEMMAP_ACPI_RECLAIMABLE;
                            break;
                        case STIVALE2_MMAP_ACPI_NVS:
                            bootConfig._mem._ranges->_type =
                              BOOT_MEMMAP_ACPI_NVS;
                            break;
                        case STIVALE2_MMAP_BAD_MEMORY:
                            bootConfig._mem._ranges->_type =
                              BOOT_MEMMAP_BAD_MEMORY;
                            break;
                        case STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE:
                            bootConfig._mem._ranges->_type =
                              BOOT_MEMMAP_BOOTLOADER_RECLAIMABLE;
                            break;
                        case STIVALE2_MMAP_RESERVED:
                            bootConfig._mem._ranges->_type =
                              BOOT_MEMMAP_RESERVED;
                            break;
                    }
                }
                break;
            }
            case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID: {
                stivale2_struct_tag_framebuffer* tagFb =
                  reinterpret_cast<stivale2_struct_tag_framebuffer*>(stTags);
                bootConfig._gfx = {
                    ._width   = tagFb->framebuffer_width,
                    ._height  = tagFb->framebuffer_height,
                    ._address = tagFb->framebuffer_addr,
                    ._pitch   = tagFb->framebuffer_pitch,
                    ._bpp     = tagFb->framebuffer_bpp,
                };
                break;
            }
        }
    }
    Kern::Main::setupKernel(&bootConfig);
}