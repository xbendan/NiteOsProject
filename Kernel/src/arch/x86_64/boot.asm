extern kload_stivale2
extern stack_top

MULTIBOOT_MAGIC       equ 0xE85250D6
MULTIBOOT_ARCH        equ 0 ; x86
MULTIBOOT_LENGTH      equ (multiboot_header_end - multiboot_header_start)
MULTIBOOT_CHECKSUM    equ -(MULTIBOOT_MAGIC + MULTIBOOT_ARCH + MULTIBOOT_LENGTH)

section .boot.text
multiboot_header_start:
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_ARCH
    dd MULTIBOOT_LENGTH
    dd MULTIBOOT_CHECKSUM

align 8
mb_tags_start:
    dw 1
    dw 0
    dd mb_tags_end - mb_tags_start
    dd 1 ; require cmd line
    dd 2 ; require bootloader name
    dd 4 ; require basic memory info
    dd 6 ; require memory map
mb_tags_end:

align 8
; Framebuffer tag
    dw 5  ; Type - framebuffer
    dw 0  ; Flags
    dd 20 ; Size - 20
    dd 0  ; Width
    dd 0  ; Height
    dd 32  ; Depth

    ; Eng tag
    dw 0
    dw 0
    dd 8
multiboot_header_end:

section .stivale2hdr
align 4
stivale2header:
    dq kload_stivale2
    dq stack_top
    dq 0
    dq 0                ; Do NOT set framebuffer here.

section .data
stivale2tag_smp:
    dq 0x1ab015085f3273df
    dq 0
    dq 1
stivale2linearframebuffer:
    dq 0xc75c9fa92a44c4db
    dq stivale2framebufferWctag
    dq 0
stivale2framebuffer: ; Framebuffer
    dq 0x3ecc1bc43d0f7971
    dq 0 ; Next tag
    dq 0 ; Width
    dq 0 ; Height
    dq 32 ; BPP
stivale2framebufferWctag: ; Ask the bootloader to set framebuffer as writecombining in MTRRs
    dq 0x4c7bb07731282e00
    dq 0 ; No next tag  

section .data
global __cxa_atexit
__cxa_atexit:
  ret

section .bss
global __dso_handle
__dso_handle: resq 1

