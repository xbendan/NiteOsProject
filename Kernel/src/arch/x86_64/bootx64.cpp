#include <siberix/init/boot.h>
#include <siberix/init/stivale2.h>

#include <siberix/mm/page.h>

static BootConfig bootConfig;
extern "C" void   KernelEntrySt2(stivale2_struct* pointer) {
    if (pointer == nullptr) {
        return;
    }

    stivale2_tag* tag = pointer->tags;
    while (tag) {
        switch (tag->identifier) {
            case STIVALE2_STRUCT_TAG_MEMMAP_ID: {
                int                         segmentId = 0;
                stivale2_struct_tag_memmap* mmap =
                    static_cast<stivale2_struct_tag_memmap*>(tag);
                auto getEntryType = [](u32 entry) -> PageBlockType {
                    switch (entry) {
                        case STIVALE2_MMAP_USABLE:
                            return BlkTypeAvailable;
                        case STIVALE2_MMAP_RESERVED:
                            return BlkTypeReserved;
                        case STIVALE2_MMAP_KERNEL_AND_MODULES:
                            return BlkTypeKernel;
                        case STIVALE2_MMAP_BAD_MEMORY:
                            return BlkTypeBadram;
                        case STIVALE2_MMAP_ACPI_NVS:
                            return BlkTypeReserved;
                        case STIVALE2_MMAP_ACPI_RECLAIMABLE:
                            return BlkTypeAvailable;
                        case STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE:
                            return BlkTypeAvailable;
                        default:
                            return BlkTypeReserved;
                    }
                };
                for (u64 index = 0; index < mmap->entries; index++) {
                    stivale2_mmap_entry* entry = &mmap->entries[index];
                    u64                  start = entry->base;
                    u64                  end   = entry->base + entry->end;
                    if (end > bootConfig.memory.maxSize) {
                        bootConfig.memory.maxSize = end;
                    }
                    bootConfig.memory.ranges[segmentId++] =
                        PageBlock(start, end, getEntryType(entry->type));
                }
            }
            case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID: {
                stivale2_struct_tag_framebuffer* fb =
                    static_cast<stivale2_struct_tag_framebuffer*>(tag);
                bootConfig.graphic = { .width   = fb->framebuffer_width,
                                         .height  = fb->framebuffer_height,
                                         .address = fb->framebuffer_addr,
                                         .pitch   = fb->framebuffer_pitch,
                                         .bpp     = fb_framebuffer_bpp };
            }
        }
        tag = reinterpret_cast<stivale2_tag*>(tag->next);
    }

    kmain(bootConfig);
}