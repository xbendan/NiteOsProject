global asmw_flush_gdt
global asmw_flush_idt
global asmw_load_paging
global asmw_enable_sse
global asmw_enable_avx
global asmw_get_pagemap

asmw_flush_gdt:
    lgdt [rdi]
    push rbp
    mov rbp, rsp

    push qword 0x10
    push rbp
    pushf
    push qword 0x8
    push .trampoline
    iretq
.trampoline:
    pop rbp

    mov ax, 0x10

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ax, 0x28
    ltr ax

    ret

asmw_flush_idt:
    lidt [rdi]
    ret

asmw_load_paging:
    mov cr3, rdi
    ret

asmw_enable_sse:
    mov rcx, 200h
    mov rbx, cr4
    or rbx, rcx
    mov cr4, rbx

    fninit

    ret

asmw_enable_avx:
    push rax
    push rcx
    push rdx

    xor rcx, rcx
    xgetbv
    or eax, 7
    xsetbv

    pop rdx
    pop rcx
    pop rax
    
    ret

bits 32
asmw_get_pagemap:
    mov eax, cr3
    ret