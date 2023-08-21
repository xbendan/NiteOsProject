extern fDispatchInterrupts
extern fSystemCall
global intSyscall
global intTables

section .text

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

%macro popaq 0
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

%macro fIntNoErr 1
global int%1
int%1:
    cli
    push 0
    pushaq
    mov rdi, %1
    mov rsi, rsp
    xor rbp, rbp
    call fDispatchInterrupts
    popaq
    add rsp, 8
    iretq
%endmacro

%macro fIntErr 1
global int%1
int%1:
    cli
    pushaq
    mov rdi, %1
    mov rsi, rsp
    xor rbp, rbp
    call fDispatchInterrupts
    popaq
    add rsp, 8
    iretq
%endmacro

%macro fIntIRQ 1
global int%1
int%1:
    cli
    push 0
    pushaq
    mov rdi, %1
    mov rsi, rsp
    xor rdx, rdx
    xor rbp, rbp
    call fDispatchInterrupts
    popaq
    add rsp, 8
    iretq
%endmacro

%macro fIntIPI 1
global int%1
int%1:
    cli
    push 0
    pushaq
    mov rdi, %1
    mov rsi, rsp
    xor rbp, rbp
    call fDispatchInterrupts
    popaq
    add rsp, 8
    iretq
%endmacro

fIntNoErr  0
fIntNoErr  1
fIntNoErr  2
fIntNoErr  3
fIntNoErr  4
fIntNoErr  5
fIntNoErr  6
fIntNoErr  7
fIntErr 8
fIntNoErr  9
fIntErr 10
fIntErr 11
fIntErr 12
fIntErr 13
fIntErr 14
fIntNoErr  15
fIntNoErr  16
fIntErr  17
fIntNoErr  18
fIntNoErr 19
fIntNoErr 20
fIntNoErr 21
fIntNoErr 22
fIntNoErr 23
fIntNoErr 24
fIntNoErr 25
fIntNoErr 26
fIntNoErr 27
fIntNoErr 28
fIntNoErr 29
fIntErr 30
fIntNoErr 31

%assign irqn 32
%rep 16
    fIntIRQ irqn
    %assign irqn irqn+1
%endrep

%assign ipin 48
%rep (256 - 48)
    fIntIPI ipin
    %assign ipin (ipin + 1)
%endrep

intSyscall:
    cli
    push 0
    pushaq
    mov rsi, rsp
    xor rbp, rbp
    call fSystemCall
    popaq
    add rsp, 8
    iretq

section .rodata
intTables:
%assign i 48
%rep (256 - 48)
    dq (int%+i)
    %assign i (i + 1)
%endrep