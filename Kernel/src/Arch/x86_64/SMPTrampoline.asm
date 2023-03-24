global SMPTrampolineStart
global SMPTrampolineEnd
SMP_TRAMPOLINE_ENTRY64 equ (SMPTrampolineStart64 - SMPTrampolineStart + SMP_TRAMPOLINE_ENTRY)

SMP_TRAMPOLINE_ENTRY equ 0x2000
SMP_TRAMPOLINE_DATA_ADDR equ 0x1000
SMP_TRAMPOLINE_DATA_START_FLAG equ SMP_TRAMPOLINE_DATA_ADDR
SMP_TRAMPOLINE_CPU_ID equ SMP_TRAMPOLINE_DATA_ADDR + 0x2
SMP_TRAMPOLINE_GDT_PTR equ SMP_TRAMPOLINE_DATA_ADDR + 0x10
SMP_TRAMPOLINE_CR3 equ SMP_TRAMPOLINE_DATA_ADDR + 0x20
SMP_TRAMPOLINE_STACK equ SMP_TRAMPOLINE_DATA_ADDR + 0x28
SMP_TRAMPOLINE_ENTRY2 equ SMP_TRAMPOLINE_DATA_ADDR + 0x30
SMP_MAGIC equ 0xB33F

section .text
bits 16
SMPTrampolineStart:

cli
cld

mov ax, SMP_MAGIC
mov word [SMP_TRAMPOLINE_DATA_START_FLAG], ax

mov eax, cr4
or eax, 1 << 5  ; Set PAE bit
mov cr4, eax

mov eax, dword [SMP_TRAMPOLINE_CR3]
mov cr3, eax

mov ecx, 0xC0000080 ; EFER Model Specific Register
rdmsr               ; Read from the MSR 
or eax, 1 << 8
or eax, 1           ; Enable Syscall
wrmsr

mov eax, cr0
or eax, 0x80000001  ; Paging, Protected Mode
mov cr0, eax

lgdt [SMP_TRAMPOLINE_GDT_PTR]

jmp 0x08:(SMP_TRAMPOLINE_ENTRY64)

hlt

bits 64
SMPTrampolineStart64:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rsp, [SMP_TRAMPOLINE_STACK]

    mov rax, cr0
	and ax, 0xFFFB		; Clear coprocessor emulation
	or ax, 0x2			; Set coprocessor monitoring
	mov cr0, rax

	;Enable SSE
	mov rax, cr4
	or ax, 3 << 9		; Set flags for SSE
	mov cr4, rax

    xor rbp, rbp
    mov rdi, [SMP_TRAMPOLINE_CPU_ID]

    call [SMP_TRAMPOLINE_ENTRY2]

    cli
    hlt
SMPTrampolineEnd: