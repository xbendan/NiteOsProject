global SMPTrampolineStart
global SMPTrampolineEnd

SMPTrampolineStart_RealMode:
    cli
    mov ax, 0x0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
SMPTrampolineEnd_RealMode:

SMPTrampolineStart_ProtectedMode:
SMPTrampolineEnd_ProtectedMode:

SMPTrampolineStart_LongMode:
SMPTrampolineEnd_LongMode:  