section .text

_MKTAS_CALC_TAN:
    movsd xmm7, xmm0
    call _MKTAS_CALC_COS
    movsd xmm8, xmm0
    movsd xmm0, xmm7
    call _MKTAS_CALC_SIN
    divsd xmm0, xmm8
    ret