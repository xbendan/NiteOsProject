OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(start_mb32)

SECTIONS {

    KERNEL_VMA = 0xFFFFFFFF80000000;
    . = 0x100000;

    .boot.text : 
    {
        KEEP(*(.boot.text))
    }

    .boot.data :
    {
        KEEP(*(.boot.data))
        . = ALIGN(4096);
    }

    . += KERNEL_VMA;
    KERNEL_START_ADDR = .;
    .stivalehdr ALIGN(4K) :
    {
        KEEP(*(.stivalehdr))
    }

    .stivale2hdr ALIGN(4K) : AT(ADDR(.stivale2hdr) - KERNEL_VMA)
    {
        KEEP(*(.stivale2hdr))
    }

    .text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VMA)
    {
        KEEP(*(.text*))
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VMA)
    {
        KEEP(*(.rodata*))
    }

    .data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VMA)
    {
        KEEP(*(.data*))
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VMA)
    {
        __bss = .;
        KEEP(*(COMMON))
        KEEP(*(.bss*))
        __bss_end = .;
    }

    KERNEL_END_ADDR = .;

    /DISCARD/ :
    {
        *(.comment)
    }
}