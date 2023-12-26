extern kload_st2

section .bss
align 0x10
stackBottom:
resb 0x10000
stackTop:

section .st2hdr
align 0x4
headerSt2:
    dq kload_st2
    dq stackTop
    dq 0
    dq 0

section .data
st2FeatSMP:
    dq 0x1ab015085f3273df
    dq 0
    dq 1
st2FeatLinearFB:
    dq 0xc75c9fa92a44c4db
    dq 0
    dq 1
st2FeatFB:
    dq 0x3ecc1bc43d0f7971
    dq 0
    dq 0 ; Width
    dq 0 ; Height
    dq 32 ; Bits per pixel
    