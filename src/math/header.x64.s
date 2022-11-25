DEFAULT REL
SECTION .TEXT
	GLOBAL _MKTAS_STAT
    GLOBAL _MKTAS_UIFAC
    GLOBAL _MKTAS_CALC_E
    GLOBAL _MKTAS_CALC_EXP
    GLOBAL _MKTAS_CALC_SINH
    GLOBAL _MKTAS_CALC_COSH
    GLOBAL _MKTAS_CALC_TANH
    GLOBAL _MKTAS_CALC_SIN
    GLOBAL _MKTAS_CALC_COS
    GLOBAL _MKTAS_CALC_TAN

%include "./src/math/0.x64.stat.s"
%include "./src/math/1.x64.uifac.s"
%include "./src/math/2.x64.e.s"
%include "./src/math/3.x64.exp.s"
%include "./src/math/4.x64.sinh.s"
%include "./src/math/5.x64.cosh.s"
%include "./src/math/6.x64.tanh.s"
%include "./src/math/7.x64.sin.s"
%include "./src/math/8.x64.cos.s"
%include "./src/math/9.x64.tan.s"
