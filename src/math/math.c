#include <MKTAppEngine.h>

// extern _MKTAS_CALC_E();

unsigned long long int _MKTGENERALFUNC(IntDex index, void * data)
{
    switch(index)
    {
        case 0:
        return (unsigned long long int)_MKTAS_STAT(data);
    }
}