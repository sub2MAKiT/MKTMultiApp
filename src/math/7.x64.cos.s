section .text

_MKTAS_CALC_COS:
    ; xmm0 has the input
    ; xmm0 has the output
    ; In here It might be quicker, if I just hardcode everything and go for the: speed > memory (memory in here will mean around a kilobyto, so I'm good)
    movsd xmm1, xmm0
    movsd xmm4, xmm0

    mov rax, 6
    mulsd xmm1, xmm4
    mulsd xmm1, xmm4
    movsd xmm2, xmm1
    cvtsi2sd xmm3, rax
    divsd xmm1, xmm3
    subsd xmm0, xmm1
    movsd xmm1, xmm2

    mov rax, 120
    mulsd xmm1, xmm4
    mulsd xmm1, xmm4
    movsd xmm2, xmm1
    cvtsi2sd xmm3, rax
    divsd xmm1, xmm3
    addsd xmm0, xmm1
    movsd xmm1, xmm2

    mov rax, 5040
    mulsd xmm1, xmm4
    mulsd xmm1, xmm4
    movsd xmm2, xmm1
    cvtsi2sd xmm3, rax
    divsd xmm1, xmm3
    subsd xmm0, xmm1
    movsd xmm1, xmm2

    mov rax, 362880
    mulsd xmm1, xmm4
    mulsd xmm1, xmm4
    movsd xmm2, xmm1
    cvtsi2sd xmm3, rax
    divsd xmm1, xmm3
    addsd xmm0, xmm1
    movsd xmm1, xmm2

    mov rax, 39916800
    mulsd xmm1, xmm4
    mulsd xmm1, xmm4
    movsd xmm2, xmm1
    cvtsi2sd xmm3, rax
    divsd xmm1, xmm3
    subsd xmm0, xmm1
    movsd xmm1, xmm2

    mov rax, 6227020800
    mulsd xmm1, xmm4
    mulsd xmm1, xmm4
    movsd xmm2, xmm1
    cvtsi2sd xmm3, rax
    divsd xmm1, xmm3
    addsd xmm0, xmm1
    movsd xmm1, xmm2

    mov rax, 1307674368000
    mulsd xmm1, xmm4
    mulsd xmm1, xmm4
    movsd xmm2, xmm1
    cvtsi2sd xmm3, rax
    divsd xmm1, xmm3
    subsd xmm0, xmm1
    movsd xmm1, xmm2

    ret