[bits 64]
global LoadGDT
LoadGDT: 
    lgdt [rdi]
    mov ax, 0x28
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    pop rdi
    mov rax, 0x30
    push rax
    push rdi
    retfq

disablePIC: 
    mov al, 0xff
    out 0xa1, al
    out 0x21, al
    ret
GLOBAL disablePIC