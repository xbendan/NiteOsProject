global stack_bottom
global stack_top

section .bss
align 16
stack_bottom:
resb 4096 * 8 ; 32 KiB
stack_top:
