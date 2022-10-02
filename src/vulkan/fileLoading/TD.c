#include <fileLoading/TD.h>

IntDex _MKT_genTD(MKTTDdata input)
{

}

MKTtd _MKT_openTD(char * FP)
{

}

IntDex _MKT_loadTD(char * FP)
{
    MKTtd loaded = _MKT_openTD(FP);

    return _MKT_genTD(loaded._dataPiC);
}