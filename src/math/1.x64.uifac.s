SECTION .TEXT
	GLOBAL _MKTAS_UIFAC

_MKTAS_UIFAC:
    mov rax, rcx
backLabel:
    cmp rcx, 1
    jne loopAndMul
    ret

loopAndMul:
    dec rcx
    imul rax, rcx
    jmp backLabel