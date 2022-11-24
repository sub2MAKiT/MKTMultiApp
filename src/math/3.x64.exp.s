section .bss
    one3: resq 1
section .text

;+------------------+
;|  xmm0 - output   |
;|  xmm1 - power    |
;|  xmm3 - input    |
;|  rbx - iter      |
;|  rcx - iter cap  |
;|  rdx - factiter  |
;+------------------+

_MKTAS_CALC_EXP:
    ; xmm0 has the second input
    ; xmm0 has the output
    ; rcx has the first input
    ; init phase
    mov [one3], dword 1
    movsd xmm3, xmm0
    xor rbx, rbx
    mov rdx, 1
    cvtsi2sd xmm1, [one3]
    cvtsi2sd xmm0, [one3]
    ; end of the init phase
topOfMainIter3:
    inc rbx
    cmp rbx, rcx
    jne botOfMainIter3
    ret

botOfMainIter3:
    mulsd xmm1, xmm3
    mov rax, rdx
    mul rbx
    mov rdx, rax
    cvtsi2sd xmm2, rdx
    movsd xmm4, xmm1
    divsd xmm4, xmm2
    addsd xmm0, xmm4
    jmp topOfMainIter3


    ; add rcx, rdx
    ; mov , 
    ; mov rax, rcx
    ; cvtsi2sd xmm0, rcx
    ; mov [temp3], xmm1
    ; movq rax, xmm1
    ; xorps xmm0, xmm0
    ; ret
;     movsd [in3], XMM0
;     movsd XMM1, [in3]
;     mov rax, 1
;     mov rdx, 1
; mainIterTop3:
;     addsd XMM0, XMM1
;     movsd XMM1, [in3]
;     cmp rax, rcx
;     jne mainIterBot3

; mainIterBot3:
;     inc rax
;     imul rdx, rax
;     cvtsi2sd xmm2, rdx
;     divsd XMM1, xmm2
;     jmp mainIterTop3