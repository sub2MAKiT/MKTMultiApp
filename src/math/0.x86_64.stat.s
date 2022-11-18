SECTION .TEXT
	GLOBAL _MKTAS_STAT

_MKTAS_STAT:
    mov  rax, [rsi]
    ret                        ; Return control