section .text

_MKTAS_CALC_SINH:
    ; xmm0 has the second input
    ; xmm0 has the output
    ; rcx has the first input
    ; init phase
    call _MKTAS_CALC_EXP
    xor rax, rax
    inc rax
    cvtsi2sd xmm1, rax
    movsd xmm2, xmm0
    divsd xmm1, xmm2
    subsd xmm0, xmm1
    inc rax
    cvtsi2sd xmm1, rax
    divsd xmm0, xmm1
    ret