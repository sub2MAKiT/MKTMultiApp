#include <fileLoading/fileLoading.h>

IntDex loadFile(char * FP, char type)
{
    IntDex returnValue = 0;

    if(type == MKTINFOV)
        0;
    else if(type == MKTAGV)
        _MKT_loadAG(FP);
    else if(type == MKTPICV)
        _MKT_loadPiC(FP);
    else if(type == MKTAUDIOV)
        0;
    else if(type == MKTVIDV)
        0;
    else if(type == MKTFONTPV)
        0;
    else if(type == MKTFONTMV)
        0;
    else if(type == MKTOBJV)
        _MKT_loadTD(FP);
    else
        0;

    return returnValue;
}