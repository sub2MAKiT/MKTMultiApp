_MKTAS_UIFAC:
    mov rax, rcx
backLabel1:
    cmp rcx, 1
    jne loopAndMul1
    ret

loopAndMul1:
    dec rcx
    imul rax, rcx
    jmp backLabel1