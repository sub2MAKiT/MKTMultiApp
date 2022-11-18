section .bss
    div2: resq 1
section .text
one2: dq 1.0

_MKTAS_CALC_E:
    movsd XMM0, [one2]
    movsd XMM1, [one2]
    mov rax, 1
    mov rdx, 1
mainIterTop2:
    addsd XMM0, XMM1
    movsd XMM1, [one2]
    cmp rax, rcx
    jne mainIterBot2
    ret

mainIterBot2:
    inc rax
    imul rdx, rax
    cvtsi2sd xmm2, rdx
    divsd XMM1, xmm2
    jmp mainIterTop2