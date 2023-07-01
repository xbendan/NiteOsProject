#include <siberix/init/boot.h>
#include <siberix/init/stivale2.h>
#include <siberix/mm/segments.h>

static BootConfig bootConfig;
extern "C" void KernelEntrySt2(stivale2_struct *pointer)
{
    if (pointer == nullptr) {
        
    }
    stivale2_tag *tag 
        = pointer->tags;
    while (tag)
    {
        switch (tag->identifier) {
            case STIVALE2_STRUCT_TAG_MEMMAP_ID: {
                stivale2_struct_tag_memmap *mmap 
                    = static_cast<stivale2_struct_tag_memmap *>(tag);
                auto getEntryType = [](u32 entry) -> AddressSegmentType
                {
                    switch (entry)
                    {
                    case STIVALE2_MMAP_USABLE:
                        return AST_AVAILABLE;
                    case STIVALE2_MMAP_RESERVED:
                        return AST_RESERVED;
                    case STIVALE2_MMAP_KERNEL_AND_MODULES:
                        return AST_KERNEL;
                    case STIVALE2_MMAP_BAD_MEMORY:
                        return AST_BADRAM;
                    case STIVALE2_MMAP_ACPI_NVS:
                        return AST_RESERVED;
                    case STIVALE2_MMAP_ACPI_RECLAIMABLE:
                        return AST_AVAILABLE;
                    case STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE:
                        return AST_AVAILABLE;
                    default:
                        return AST_RESERVED;
                    }
                };
                for (u64 index = 0; index < mmap->entries; index++) {
                    stivale2_mmap_entry *entry = &mmap->entries[index];
                    addressSegments.add(AddressSegment(
                        entry->base,
                        entry->base + entry->length,
                        getEntryType(entry->type)));
                }
            }
            case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID: {
                stivale2_struct_tag_framebuffer *fb 
                    = static_cast<stivale2_struct_tag_framebuffer *>(tag);
                bootConfig.graphic = {
                    .width = fb->framebuffer_width,
                    .height = fb->framebuffer_height,
                    .address = fb->framebuffer_addr,
                    .pitch = fb->framebuffer_pitch,
                    .bpp = fb_framebuffer_bpp
                };
            }
        }
        tag = reinterpret_cast<stivale2_tag *>(tag->next);
    }
}