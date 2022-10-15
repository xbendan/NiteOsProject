/*  multiboot2.h - Multiboot 2 header file.  */
/*  Copyright (C) 1999,2003,2007,2008,2009,2010  Free Software Foundation, Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL ANY
 *  DEVELOPER OR DISTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdint.h>

#ifndef MULTIBOOT_HEADER
#define MULTIBOOT_HEADER 1

/* How many bytes from the start of the file we search for the header.  */
#define MULTIBOOT_SEARCH			32768
#define MULTIBOOT_HEADER_ALIGN			8

/* The magic field should contain this.  */
#define MULTIBOOT2_HEADER_MAGIC			0xe85250d6

/* This should be in %eax.  */
#define MULTIBOOT2_BOOTLOADER_MAGIC		0x36d76289

/* Alignment of multiboot modules.  */
#define MULTIBOOT_MOD_ALIGN			0x00001000

/* Alignment of the multiboot info structure.  */
#define MULTIBOOT_INFO_ALIGN			0x00000008

/* Flags set in the 'flags' member of the multiboot header.  */

#define MULTIBOOT_TAG_ALIGN                  8
#define MULTIBOOT_TAG_TYPE_END               0
#define MULTIBOOT_TAG_TYPE_CMDLINE           1
#define MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME  2
#define MULTIBOOT_TAG_TYPE_MODULE            3
#define MULTIBOOT_TAG_TYPE_BASIC_MEMINFO     4
#define MULTIBOOT_TAG_TYPE_BOOTDEV           5
#define MULTIBOOT_TAG_TYPE_MMAP              6
#define MULTIBOOT_TAG_TYPE_VBE               7
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER       8
#define MULTIBOOT_TAG_TYPE_ELF_SECTIONS      9
#define MULTIBOOT_TAG_TYPE_APM               10
#define MULTIBOOT_TAG_TYPE_EFI32             11
#define MULTIBOOT_TAG_TYPE_EFI64             12
#define MULTIBOOT_TAG_TYPE_SMBIOS            13
#define MULTIBOOT_TAG_TYPE_ACPI_OLD          14
#define MULTIBOOT_TAG_TYPE_ACPI_NEW          15
#define MULTIBOOT_TAG_TYPE_NETWORK           16
#define MULTIBOOT_TAG_TYPE_EFI_MMAP          17
#define MULTIBOOT_TAG_TYPE_EFI_BS            18
#define MULTIBOOT_TAG_TYPE_EFI32_IH          19
#define MULTIBOOT_TAG_TYPE_EFI64_IH          20
#define MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR    21

#define MULTIBOOT_HEADER_TAG_END  0
#define MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST  1
#define MULTIBOOT_HEADER_TAG_ADDRESS  2
#define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS  3
#define MULTIBOOT_HEADER_TAG_CONSOLE_FLAGS  4
#define MULTIBOOT_HEADER_TAG_FRAMEBUFFER  5
#define MULTIBOOT_HEADER_TAG_MODULE_ALIGN  6
#define MULTIBOOT_HEADER_TAG_EFI_BS  7
#define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64  9
#define MULTIBOOT_HEADER_TAG_RELOCATABLE  10

#define MULTIBOOT2_ARCHITECTURE_I386  0
#define MULTIBOOT2_ARCHITECTURE_MIPS32  4
#define MULTIBOOT_HEADER_TAG_OPTIONAL 1

#define MULTIBOOT_LOAD_PREFERENCE_NONE 0
#define MULTIBOOT_LOAD_PREFERENCE_LOW 1
#define MULTIBOOT_LOAD_PREFERENCE_HIGH 2

#define MULTIBOOT_CONSOLE_FLAGS_CONSOLE_REQUIRED 1
#define MULTIBOOT_CONSOLE_FLAGS_EGA_TEXT_SUPPORTED 2

#ifndef ASM_FILE

typedef unsigned char		multiboot_uint8_t;
typedef unsigned short		multiboot_uint16_t;
typedef unsigned int		multiboot_uint32_t;
typedef unsigned long long	multiboot_uint64_t;

struct multiboot_header
{
  /* Must be MULTIBOOT_MAGIC - see above.  */
  multiboot_uint32_t magic;

  /* ISA */
  multiboot_uint32_t architecture;

  /* Total header length.  */
  multiboot_uint32_t header_length;

  /* The above fields plus this one must equal 0 mod 2^32. */
  multiboot_uint32_t checksum;
};

struct multiboot_header_tag
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
};

struct multiboot_header_tag_information_request
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
  multiboot_uint32_t requests[0];
};

struct multiboot_header_tag_address
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
  multiboot_uint32_t header_addr;
  multiboot_uint32_t load_addr;
  multiboot_uint32_t load_end_addr;
  multiboot_uint32_t bss_end_addr;
};

struct multiboot_header_tag_entry_address
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
  multiboot_uint32_t entry_addr;
};

struct multiboot_header_tag_console_flags
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
  multiboot_uint32_t console_flags;
};

struct multiboot_header_tag_framebuffer
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
  multiboot_uint32_t width;
  multiboot_uint32_t height;
  multiboot_uint32_t depth;
};

struct multiboot_header_tag_module_align
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
};

struct multiboot_header_tag_relocatable
{
  multiboot_uint16_t type;
  multiboot_uint16_t flags;
  multiboot_uint32_t size;
  multiboot_uint32_t min_addr;
  multiboot_uint32_t max_addr;
  multiboot_uint32_t align;
  multiboot_uint32_t preference;
};

struct multiboot_tag
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
};

typedef struct{
	multiboot_uint32_t totalSize;
	multiboot_uint32_t reserved;
	struct multiboot_tag tags[0];
} __attribute__((packed)) multiboot2_info_header_t;

struct multiboot_color
{
  multiboot_uint8_t red;
  multiboot_uint8_t green;
  multiboot_uint8_t blue;
};

struct multiboot_mmap_entry
{
  multiboot_uint64_t addr;
  multiboot_uint64_t length;
#define MULTIBOOT_MEMORY_AVAILABLE		1
#define MULTIBOOT_MEMORY_RESERVED		2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5
  multiboot_uint32_t type;
  multiboot_uint32_t zero;
};
typedef struct multiboot_mmap_entry multiboot_memory_map_t;

struct multiboot_tag_string
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  char string[0];
};

struct multiboot_tag_module
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t mod_start;
  multiboot_uint32_t mod_end;
  char cmdline[0];
};

struct multiboot_tag_basic_meminfo
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t mem_lower;
  multiboot_uint32_t mem_upper;
} multiboot2_memory_info_t;

struct multiboot_tag_bootdev
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t biosdev;
  multiboot_uint32_t slice;
  multiboot_uint32_t part;
};

struct multiboot_tag_mmap
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t entry_size;
  multiboot_uint32_t entry_version;
  struct multiboot_mmap_entry entries[0];  
};

struct multiboot_vbe_info_block
{
  multiboot_uint8_t external_specification[512];
};

struct multiboot_vbe_mode_info_block
{
  multiboot_uint8_t external_specification[256];
};

struct multiboot_tag_vbe
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;

  multiboot_uint16_t vbe_mode;
  multiboot_uint16_t vbe_interface_seg;
  multiboot_uint16_t vbe_interface_off;
  multiboot_uint16_t vbe_interface_len;

  struct multiboot_vbe_info_block vbe_control_info;
  struct multiboot_vbe_mode_info_block vbe_mode_info;
};

struct multiboot_tag_framebuffer_common
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;

  multiboot_uint64_t framebuffer_addr;
  multiboot_uint32_t framebuffer_pitch;
  multiboot_uint32_t framebuffer_width;
  multiboot_uint32_t framebuffer_height;
  multiboot_uint8_t framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB     1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT	2
  multiboot_uint8_t framebuffer_type;
  multiboot_uint16_t reserved;
};

struct multiboot_tag_framebuffer
{
  struct multiboot_tag_framebuffer_common common;

  union
  {
    struct
    {
      multiboot_uint16_t framebuffer_palette_num_colors;
      struct multiboot_color framebuffer_palette[0];
    };
    struct
    {
      multiboot_uint8_t framebuffer_red_field_position;
      multiboot_uint8_t framebuffer_red_mask_size;
      multiboot_uint8_t framebuffer_green_field_position;
      multiboot_uint8_t framebuffer_green_mask_size;
      multiboot_uint8_t framebuffer_blue_field_position;
      multiboot_uint8_t framebuffer_blue_mask_size;
    };
  };
};

struct multiboot_tag_elf_sections
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t num;
  multiboot_uint32_t entsize;
  multiboot_uint32_t shndx;
  char sections[0];
};

struct multiboot_tag_apm
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint16_t version;
  multiboot_uint16_t cseg;
  multiboot_uint32_t offset;
  multiboot_uint16_t cseg_16;
  multiboot_uint16_t dseg;
  multiboot_uint16_t flags;
  multiboot_uint16_t cseg_len;
  multiboot_uint16_t cseg_16_len;
  multiboot_uint16_t dseg_len;
};

struct multiboot_tag_efi32
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t pointer;
};

struct multiboot_tag_efi64
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint64_t pointer;
};

struct multiboot_tag_smbios
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint8_t major;
  multiboot_uint8_t minor;
  multiboot_uint8_t reserved[6];
  multiboot_uint8_t tables[0];
};

struct multiboot_tag_old_acpi
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint8_t rsdp[0];
};

struct multiboot_tag_new_acpi
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint8_t rsdp[0];
};

struct multiboot_tag_network
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint8_t dhcpack[0];
};

struct multiboot_tag_efi_mmap
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t descr_size;
  multiboot_uint32_t descr_vers;
  multiboot_uint8_t efi_mmap[0];
}; 

struct multiboot_tag_efi32_ih
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t pointer;
};

struct multiboot_tag_efi64_ih
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint64_t pointer;
};

struct multiboot_tag_load_base_addr
{
  multiboot_uint32_t type;
  multiboot_uint32_t size;
  multiboot_uint32_t load_base_addr;
};
#endif /* ! ASM_FILE */
#endif /* ! MULTIBOOT_HEADER */



#define STIVALE2_HEADER_MAGIC 0x73747632

#ifndef __STIVALE__STIVALE2_H__
#define __STIVALE__STIVALE2_H__

#if (defined (_STIVALE2_SPLIT_64) && defined (__i386__)) || defined(_STIVALE2_SPLIT_64_FORCE)

#define _stivale2_split64(NAME) \
    union {                    \
        uint32_t NAME;         \
        uint32_t NAME##_lo;    \
    };                         \
    uint32_t NAME##_hi

#else

#define _stivale2_split64(NAME) \
    uint64_t NAME

#endif

// Anchor for non ELF kernels
struct stivale2_anchor {
    uint8_t anchor[15];
    uint8_t bits;
    _stivale2_split64(phys_load_addr);
    _stivale2_split64(phys_bss_start);
    _stivale2_split64(phys_bss_end);
    _stivale2_split64(phys_stivale2hdr);
};

struct stivale2_tag {
    uint64_t identifier;
    _stivale2_split64(next);
};

/* --- Header --------------------------------------------------------------- */
/*  Information passed from the kernel to the bootloader                      */

struct stivale2_header {
    _stivale2_split64(entry_point);
    _stivale2_split64(stack);
    uint64_t flags;
    _stivale2_split64(tags);
};

#define STIVALE2_HEADER_TAG_ANY_VIDEO_ID 0xc75c9fa92a44c4db

struct stivale2_header_tag_any_video {
    struct stivale2_tag tag;
    uint64_t preference;
};

#define STIVALE2_HEADER_TAG_FRAMEBUFFER_ID 0x3ecc1bc43d0f7971

struct stivale2_header_tag_framebuffer {
    struct stivale2_tag tag;
    uint16_t framebuffer_width;
    uint16_t framebuffer_height;
    uint16_t framebuffer_bpp;
    uint16_t unused;
};

#define STIVALE2_HEADER_TAG_FB_MTRR_ID 0x4c7bb07731282e00

#define STIVALE2_HEADER_TAG_SLIDE_HHDM_ID 0xdc29269c2af53d1d

struct stivale2_header_tag_slide_hhdm {
    struct stivale2_tag tag;
    uint64_t flags;
    _stivale2_split64(alignment);
};

#define STIVALE2_HEADER_TAG_TERMINAL_ID 0xa85d499b1823be72

struct stivale2_header_tag_terminal {
    struct stivale2_tag tag;
    uint64_t flags;
    _stivale2_split64(callback);
};

#define STIVALE2_TERM_CB_DEC 10
#define STIVALE2_TERM_CB_BELL 20
#define STIVALE2_TERM_CB_PRIVATE_ID 30
#define STIVALE2_TERM_CB_STATUS_REPORT 40
#define STIVALE2_TERM_CB_POS_REPORT 50
#define STIVALE2_TERM_CB_KBD_LEDS 60
#define STIVALE2_TERM_CB_MODE 70
#define STIVALE2_TERM_CB_LINUX 80

#define STIVALE2_TERM_CTX_SIZE ((uint64_t)(-1))
#define STIVALE2_TERM_CTX_SAVE ((uint64_t)(-2))
#define STIVALE2_TERM_CTX_RESTORE ((uint64_t)(-3))
#define STIVALE2_TERM_FULL_REFRESH ((uint64_t)(-4))

#define STIVALE2_HEADER_TAG_SMP_ID 0x1ab015085f3273df

struct stivale2_header_tag_smp {
    struct stivale2_tag tag;
    uint64_t flags;
};

#define STIVALE2_HEADER_TAG_5LV_PAGING_ID 0x932f477032007e8f

#define STIVALE2_HEADER_TAG_UNMAP_NULL_ID 0x92919432b16fe7e7

/* --- Struct --------------------------------------------------------------- */
/*  Information passed from the bootloader to the kernel                      */

typedef struct stivale2_struct {
#define STIVALE2_BOOTLOADER_BRAND_SIZE 64
    char bootloader_brand[STIVALE2_BOOTLOADER_BRAND_SIZE];

#define STIVALE2_BOOTLOADER_VERSION_SIZE 64
    char bootloader_version[STIVALE2_BOOTLOADER_VERSION_SIZE];

    uint64_t tags;
} stivale2_struct_t;

#define STIVALE2_STRUCT_TAG_PMRS_ID 0x5df266a64047b6bd

#define STIVALE2_PMR_EXECUTABLE ((uint64_t)1 << 0)
#define STIVALE2_PMR_WRITABLE   ((uint64_t)1 << 1)
#define STIVALE2_PMR_READABLE   ((uint64_t)1 << 2)

struct stivale2_pmr {
    uint64_t base;
    uint64_t length;
    uint64_t permissions;
};

struct stivale2_struct_tag_pmrs {
    struct stivale2_tag tag;
    uint64_t entries;
    struct stivale2_pmr pmrs[];
};

#define STIVALE2_STRUCT_TAG_KERNEL_BASE_ADDRESS_ID 0x060d78874a2a8af0

struct stivale2_struct_tag_kernel_base_address {
    struct stivale2_tag tag;
    uint64_t physical_base_address;
    uint64_t virtual_base_address;
};

#define STIVALE2_STRUCT_TAG_CMDLINE_ID 0xe5e76a1b4597a781

struct stivale2_struct_tag_cmdline {
    struct stivale2_tag tag;
    uint64_t cmdline;
};

#define STIVALE2_STRUCT_TAG_MEMMAP_ID 0x2187f79e8612de07

#define STIVALE2_MMAP_USABLE                 1
#define STIVALE2_MMAP_RESERVED               2
#define STIVALE2_MMAP_ACPI_RECLAIMABLE       3
#define STIVALE2_MMAP_ACPI_NVS               4
#define STIVALE2_MMAP_BAD_MEMORY             5
#define STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE 0x1000
#define STIVALE2_MMAP_KERNEL_AND_MODULES     0x1001
#define STIVALE2_MMAP_FRAMEBUFFER            0x1002

struct stivale2_mmap_entry {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t unused;
};

struct stivale2_struct_tag_memmap {
    struct stivale2_tag tag;
    uint64_t entries;
    struct stivale2_mmap_entry memmap[];
};

#define STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID 0x506461d2950408fa

#define STIVALE2_FBUF_MMODEL_RGB 1

struct stivale2_struct_tag_framebuffer {
    struct stivale2_tag tag;
    uint64_t framebuffer_addr;
    uint16_t framebuffer_width;
    uint16_t framebuffer_height;
    uint16_t framebuffer_pitch;
    uint16_t framebuffer_bpp;
    uint8_t  memory_model;
    uint8_t  red_mask_size;
    uint8_t  red_mask_shift;
    uint8_t  green_mask_size;
    uint8_t  green_mask_shift;
    uint8_t  blue_mask_size;
    uint8_t  blue_mask_shift;
    uint8_t  unused;
};

#define STIVALE2_STRUCT_TAG_EDID_ID 0x968609d7af96b845

struct stivale2_struct_tag_edid {
    struct stivale2_tag tag;
    uint64_t edid_size;
    uint8_t  edid_information[];
};

#define STIVALE2_STRUCT_TAG_TEXTMODE_ID 0x38d74c23e0dca893

struct stivale2_struct_tag_textmode {
    struct stivale2_tag tag;
    uint64_t address;
    uint16_t unused;
    uint16_t rows;
    uint16_t cols;
    uint16_t bytes_per_char;
};

#define STIVALE2_STRUCT_TAG_FB_MTRR_ID 0x6bc1a78ebe871172

#define STIVALE2_STRUCT_TAG_TERMINAL_ID 0xc2b3f4c3233b0974

struct stivale2_struct_tag_terminal {
    struct stivale2_tag tag;
    uint32_t flags;
    uint16_t cols;
    uint16_t rows;
    uint64_t term_write;
    uint64_t max_length;
};

#define STIVALE2_STRUCT_TAG_MODULES_ID 0x4b6fe466aade04ce

struct stivale2_module {
    uint64_t begin;
    uint64_t end;

#define STIVALE2_MODULE_STRING_SIZE 128
    char string[STIVALE2_MODULE_STRING_SIZE];
};

struct stivale2_struct_tag_modules {
    struct stivale2_tag tag;
    uint64_t module_count;
    struct stivale2_module modules[];
};

#define STIVALE2_STRUCT_TAG_RSDP_ID 0x9e1786930a375e78

struct stivale2_struct_tag_rsdp {
    struct stivale2_tag tag;
    uint64_t rsdp;
};

#define STIVALE2_STRUCT_TAG_EPOCH_ID 0x566a7bed888e1407

struct stivale2_struct_tag_epoch {
    struct stivale2_tag tag;
    uint64_t epoch;
};

#define STIVALE2_STRUCT_TAG_FIRMWARE_ID 0x359d837855e3858c

#define STIVALE2_FIRMWARE_BIOS (1 << 0)

struct stivale2_struct_tag_firmware {
    struct stivale2_tag tag;
    uint64_t flags;
};

#define STIVALE2_STRUCT_TAG_EFI_SYSTEM_TABLE_ID 0x4bc5ec15845b558e

struct stivale2_struct_tag_efi_system_table {
    struct stivale2_tag tag;
    uint64_t system_table;
};

#define STIVALE2_STRUCT_TAG_KERNEL_FILE_ID 0xe599d90c2975584a

struct stivale2_struct_tag_kernel_file {
    struct stivale2_tag tag;
    uint64_t kernel_file;
};

#define STIVALE2_STRUCT_TAG_KERNEL_FILE_V2_ID 0x37c13018a02c6ea2

struct stivale2_struct_tag_kernel_file_v2 {
    struct stivale2_tag tag;
    uint64_t kernel_file;
    uint64_t kernel_size;
};

#define STIVALE2_STRUCT_TAG_BOOT_VOLUME_ID 0x9b4358364c19ee62

struct stivale2_guid {
    uint32_t a;
    uint16_t b;
    uint16_t c;
    uint8_t  d[8];
};

struct stivale2_struct_tag_boot_volume {
    struct stivale2_tag tag;
    uint64_t flags;
    struct stivale2_guid guid;
    struct stivale2_guid part_guid;
};

#define STIVALE2_STRUCT_TAG_KERNEL_SLIDE_ID 0xee80847d01506c57

struct stivale2_struct_tag_kernel_slide {
    struct stivale2_tag tag;
    uint64_t kernel_slide;
};

#define STIVALE2_STRUCT_TAG_SMBIOS_ID 0x274bd246c62bf7d1

struct stivale2_struct_tag_smbios {
    struct stivale2_tag tag;
    uint64_t flags;
    uint64_t smbios_entry_32;
    uint64_t smbios_entry_64;
};

#define STIVALE2_STRUCT_TAG_SMP_ID 0x34d1d96339647025

struct stivale2_smp_info {
    uint32_t processor_id;
    uint32_t lapic_id;
    uint64_t target_stack;
    uint64_t goto_address;
    uint64_t extra_argument;
};

struct stivale2_struct_tag_smp {
    struct stivale2_tag tag;
    uint64_t flags;
    uint32_t bsp_lapic_id;
    uint32_t unused;
    uint64_t cpu_count;
    struct stivale2_smp_info smp_info[];
};

#define STIVALE2_STRUCT_TAG_PXE_SERVER_INFO 0x29d1e96239247032

struct stivale2_struct_tag_pxe_server_info {
    struct stivale2_tag tag;
    uint32_t server_ip;
};

#define STIVALE2_STRUCT_TAG_MMIO32_UART 0xb813f9b8dbc78797

struct stivale2_struct_tag_mmio32_uart {
    struct stivale2_tag tag;
    uint64_t addr;
};

#define STIVALE2_STRUCT_TAG_DTB 0xabb29bd49a2833fa

struct stivale2_struct_tag_dtb {
    struct stivale2_tag tag;
    uint64_t addr;
    uint64_t size;
};

#define STIVALE2_STRUCT_TAG_HHDM_ID 0xb0ed257db18cb58f

struct stivale2_struct_tag_hhdm {
    struct stivale2_tag tag;
    uint64_t addr;
};

#undef _stivale2_split64

#endif

#include <Init/BootInfo.h>

void ParseMultiboot2Info(BootInfo *bootInfo, multiboot2_info_header_t *mb_info)
{

}

void ParseStivale2Info(BootInfo *bootInfo, stivale2_struct_t *stInfo)
{
    struct stivale2_tag* tag = (struct stivale2_tag*)(stInfo->tags);
    BootInfoMemory *memInfo = &bootInfo->mem;
    while (tag)
    {
        switch (tag->identifier)
        {
        case STIVALE2_STRUCT_TAG_MEMMAP_ID:
        {
            struct stivale2_struct_tag_memmap *st2_mem_tag = (struct stivale2_struct_tag_memmap*)(tag);
            for (uint64_t idx = 0; idx < st2_mem_tag->entries; idx++)
            {
                struct stivale2_mmap_entry *fromEntry = &st2_mem_tag->memmap[idx];
                MemoryMapEntry *newEntry = &memInfo->m_MemoryMapEntries[memInfo->m_MemoryMapSize];

                if (fromEntry->base > UINTPTR_MAX ||
                    fromEntry->base + fromEntry->length > UINTPTR_MAX)
                {
                    continue;
                }

                memInfo->m_TotalSize += fromEntry->length;
                newEntry->range.start = fromEntry->base;
                newEntry->range.end = fromEntry->base + fromEntry->length;
                switch (fromEntry->type)
                {
                case STIVALE2_MMAP_USABLE:
                    memInfo->m_Usable += fromEntry->length;
                    newEntry->m_Type = 0;
                    break; 
                case STIVALE2_MMAP_KERNEL_AND_MODULES:
                    newEntry->m_Type = 5;
                    break;
                case STIVALE2_MMAP_ACPI_RECLAIMABLE:
                    newEntry->m_Type = 2;
                    break;
                case STIVALE2_MMAP_ACPI_NVS:
                    newEntry->m_Type = 3;
                    break;
                case STIVALE2_MMAP_BAD_MEMORY:
                    newEntry->m_Type = 4;
                    break;
                default:
                    newEntry->m_Type = 1;
                    break;
                }
                memInfo->map_size++;
            }
            break;
        }
        case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID:
        {
            struct stivale2_struct_tag_framebuffer *framebuffer_tag = (struct stivale2_struct_tag_framebuffer *) tag;
            BootInfoGraphics *graphic = &bootInfo->m_Graphics;

            graphic->m_Width = framebuffer_tag->framebuffer_width;
            graphic->m_Height = framebuffer_tag->framebuffer_height;

            graphic->m_BufferAddress = framebuffer_tag->framebuffer_addr;
            graphic->m_Pitch = framebuffer_tag->framebuffer_pitch;
            graphic->m_BytesPerPixel = framebuffer_tag->framebuffer_bpp;
            break;
        }
        case STIVALE2_STRUCT_TAG_MODULES_ID:
        {
            break;
        }
        case STIVALE2_STRUCT_TAG_RSDP_ID:
        {
            break;
        }
        }
        tag = (struct stivale2_tag*)(tag->next);
    }

    bootInfo->check = 0xDEADC0DE;
}