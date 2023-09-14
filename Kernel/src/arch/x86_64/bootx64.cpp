#include <arch/x86_64/arch.h>
#include <arch/x86_64/iopt.h>
#include <arch/x86_64/kaddr.h>
#include <siberix/core/runtimes.h>
#include <siberix/init/boot.h>
#include <siberix/init/limine.h>
#include <siberix/init/stivale2.h>
#include <siberix/mm/page.h>

extern "C" [[noreturn]] void
kload_limine();

static BootConfig bootConfig;

static volatile limine_boot_time_request blReqBootTime = {
    .id       = LIMINE_BOOT_TIME_REQUEST,
    .revision = 0,
};
static volatile limine_bootloader_info_request blReqBootloaderInfo = {
    .id       = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0,
};
static volatile limine_memmap_request blReqMemoryMap = {
    .id       = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};
static volatile limine_stack_size_request blReqStackSize = {
    .id         = LIMINE_STACK_SIZE_REQUEST,
    .revision   = 0,
    .stack_size = 65536,
};
static volatile limine_entry_point_request blReqEntryPoint = {
    .id       = LIMINE_ENTRY_POINT_REQUEST,
    .revision = 0,
    .entry    = kload_limine,
};
static volatile limine_hhdm_request blReqHigherHalfKernel = {
    .id       = LIMINE_HHDM_REQUEST,
    .revision = 0,
};
static volatile limine_kernel_address_request blReqKernAddress = {
    .id       = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0,
};
// static volatile limine_framebuffer_request blReqFramebufferInfo = {
//     .id       = {0, 0, 0, 0},
//     .revision = 0,
// };

extern "C" [[noreturn]] void
kload_limine()
{
    bootConfig.blTime = blReqBootTime.response->boot_time;
    bootConfig.blName = blReqBootloaderInfo.response->name;

    limine_memmap_entry** entries            = blReqMemoryMap.response->entries;
    int                   index              = 0;
    auto                  getMemoryEntryType = [](u8 entry) -> PageBlockType {
        switch (entry) {
            case LIMINE_MEMMAP_USABLE:
                return PageBlockType::Available;
            case LIMINE_MEMMAP_KERNEL_AND_MODULES:
                return PageBlockType::KernelOrModule;
            case LIMINE_MEMMAP_BAD_MEMORY:
                return PageBlockType::Badram;
            default:
                return PageBlockType::Reserved;
        }
    };
    while (entries[index]) {
        limine_memmap_entry* entry = entries[index];
        u64                  start = entry->base;
        u64                  end   = entry->base + entry->length;
        if (end > bootConfig.memory.maxSize) {
            bootConfig.memory.maxSize = end;
        }
        bootConfig.memory.totalSize += entry->length;
        bootConfig.memory.ranges[index++] =
          PageBlock(start, end, getMemoryEntryType(entry->type));
    }

    // if (blReqFramebufferInfo.response) {
    //     index                               = 0;
    //     limine_framebuffer_response* respFb = blReqFramebufferInfo.response;
    //     while (index < respFb->framebuffer_count) {
    //         limine_framebuffer* fb    = respFb->framebuffers[index];
    //         bootConfig.graphic[index] = { .width   = fb->width,
    //                                       .height  = fb->height,
    //                                       .address =
    //                                       reinterpret_cast<u64>(fb->address),
    //                                       .pitch   = fb->pitch,
    //                                       .bpp     = fb->bpp };
    //     }
    // }

    kmain(bootConfig);

    while (true)
        asm("hlt");
}

extern "C" [[noreturn]] void
kload_stivale2(stivale2_struct* pointer)
{
    if (pointer == nullptr) {
        asm("hlt");
    }

    stivale2_tag* tag = reinterpret_cast<stivale2_tag*>(pointer->tags);
    while (tag) {
        switch (tag->identifier) {
            case STIVALE2_STRUCT_TAG_MEMMAP_ID: {
                int                         segmentId = 0;
                stivale2_struct_tag_memmap* mmap =
                  reinterpret_cast<stivale2_struct_tag_memmap*>(tag);
                auto getEntryType = [](u32 entry) -> PageBlockType {
                    switch (entry) {
                        case STIVALE2_MMAP_USABLE:
                            return PageBlockType::Available;
                        case STIVALE2_MMAP_RESERVED:
                            return PageBlockType::Reserved;
                        case STIVALE2_MMAP_KERNEL_AND_MODULES:
                            return PageBlockType::KernelOrModule;
                        case STIVALE2_MMAP_BAD_MEMORY:
                            return PageBlockType::Badram;
                        case STIVALE2_MMAP_ACPI_NVS:
                            return PageBlockType::Reserved;
                        case STIVALE2_MMAP_ACPI_RECLAIMABLE:
                            return PageBlockType::Reclaimable;
                        case STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE:
                            return PageBlockType::Reclaimable;
                        default:
                            return PageBlockType::Reserved;
                    }
                };
                for (u64 index = 0; index < mmap->entries; index++) {
                    stivale2_mmap_entry* entry = &mmap->memmap[index];
                    u64                  start = entry->base;
                    u64                  end   = entry->base + entry->length;
                    if (end > bootConfig.memory.maxSize) {
                        bootConfig.memory.maxSize = end;
                    }
                    PageBlockType blockType = getEntryType(entry->type);
                    if (blockType == PageBlockType::Available) {
                        bootConfig.memory.usableSize += entry->length;
                    }
                    bootConfig.memory.totalSize += entry->length;
                    bootConfig.memory.ranges[segmentId++] =
                      PageBlock(start, end, blockType);
                }
            }
            case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID: {
                stivale2_struct_tag_framebuffer* fb =
                  reinterpret_cast<stivale2_struct_tag_framebuffer*>(tag);
                bootConfig.graphic[0] = { .width   = fb->framebuffer_width,
                                          .height  = fb->framebuffer_height,
                                          .address = fb->framebuffer_addr,
                                          .pitch   = fb->framebuffer_pitch,
                                          .bpp     = fb->framebuffer_bpp };
            }
        }
        tag = reinterpret_cast<stivale2_tag*>(tag->next);
    }

    kmain(bootConfig);

    while (true)
        asm("hlt");
}

SbrxkrnlX64Impl::SbrxkrnlX64Impl()
  : SiberixKernel(Architecture::X86_64, bootConfig)
{
}

SbrxkrnlX64Impl::~SbrxkrnlX64Impl() {}

BootConfig&
SiberixKernel::getBootConfig()
{
    return bootConfig;
}