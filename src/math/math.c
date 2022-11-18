#include <MKTAppEngine.h>

// extern _MKTAS_CALC_E();

void * _MKTGENERALFUNC(IntDex index, void * data)
{
    if(!index)
	   _MKTAS_STAT(data); // function returns the pointed value
    else if(index == 1)
        0;// _MKTAS_CALC_E();
}


// might come in handy
// https://wiki.osdev.org/Calling_Conventions