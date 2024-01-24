%macro pushaq 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro _popaq_ 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

%macro pushargs 0
    cld
    pushaq
    mov rdi, rsp
%endmacro

%macro popargs 0
    mov rsp, rax
    _popaq_
    add rsp, 16
    iretq
%endmacro

%macro intrErr 1
isr_%1
    push %1
    cld
    pushaq
    mov rdi, rsp
    call handleIntException
    mov rsp, rax
    _popaq_
    add rsp, 16
    iretq
%endmacro

%macro intrNoErr 1
isr_%1
    push 0
    push %1
    cld
    pushaq
    mov rdi, rsp
    call handleIntException
    mov rsp, rax
    _popaq_
    add rsp, 16
    iretq
%endmacro

%macro intrReq 1
irq_%2:
    push 0
    push %2
    cld
    pushaq
    mov rdi, rsp
    call handleIntRequest
    mov rsp, rax
    _popaq_
    add rsp, 16
    iretq
%endmacro

%macro intrInterp 1
ipi_%1:
    push 0
    push %1
    cld
    pushaq
    mov rdi, rsp
    call handleIntInterprocessor
    mov rsp, rax
    _popaq_
    add rsp, 16
    iretq
%endmacro

%macro intrSyscall 1
sysc_%1:
    push 0
    push %1
    cld
    pushaq
    mov rdi, rsp
    call handleIntRequest
    mov rsp, rax
    _popaq_
    add rsp, 16
    iretq
%endmacro

section .text
extern _dispatchInterrupts
_interruptHandler:
    cld
    pushaq
    mov rdi, rsp
    call _dispatchInterrupts
    mov rsp, rax
    _popaq_
    add rsp, 16
    iretq


    