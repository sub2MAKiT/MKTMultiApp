DEFAULT REL
SECTION .TEXT
	GLOBAL _MKTAS_STAT
    GLOBAL _MKTAS_UIFAC
    GLOBAL _MKTAS_CALC_E

%include "./src/math/0.x64.stat.s"
%include "./src/math/1.x64.uifac.s"
%include "./src/math/2.x64.e.s"