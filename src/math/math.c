#include <MKTAppEngine.h>

SON _MKTGENERALFUNC(IntDex index, SIN * data)
{
    switch(index)
    {
        case 0:
        return (SON)_MKTAS_STAT(data->VP);
    }
}

// extern unsigned long long _MKTAS_STAT(unsigned long long int * rcx);
// extern unsigned long long _MKTAS_UIFAC(unsigned long long int rcx);
// extern double _MKTAS_CALC_E(unsigned long long int rcx);
// extern double _MKTAS_CALC_EXP(unsigned long long int rcx,double xmm0);
// extern double _MKTAS_CALC_SINH(unsigned long long int rcx,double xmm0);
// extern double _MKTAS_CALC_COSH(unsigned long long int rcx,double xmm0);
// extern double _MKTAS_CALC_TANH(unsigned long long int rcx,double xmm0);
// extern double _MKTAS_CALC_SIN(double xmm0);
// extern double _MKTAS_CALC_COS(double xmm0);
// extern double _MKTAS_CALC_TAN(double xmm0);