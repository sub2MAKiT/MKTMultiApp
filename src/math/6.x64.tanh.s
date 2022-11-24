section .text

_MKTAS_CALC_TANH:
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
    movsd xmm3, xmm1
    subsd xmm0, xmm1
    addsd xmm2, xmm3
    divsd xmm0, xmm2
    ret