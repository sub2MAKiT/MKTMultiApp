#include <MKTAppEngine.h>

// extern _MKTAS_CALC_E();

void * _MKTGENERALFUNC(IntDex index, void * data)
{
    if(!index)
	   _MKTAS_STAT(data); // function returns the pointed value
    else if(index == 1)
        _MKTAS_UIFAC(*(unsigned long long int *)data);
    else if(index == 2)
        _MKTAS_CALC_E(*(unsigned long long int *)data);
}


// might come in handy
// https://wiki.osdev.org/Calling_Conventions