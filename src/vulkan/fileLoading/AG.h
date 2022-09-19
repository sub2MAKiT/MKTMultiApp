#include <MKTAppEngine.h>
#include <structs.h>

MKTag _MKT_openAG(char * FP);
IntDex _MKT_genAG(AGVertex * inVertices,IntDex inSizeOfVertices,unsigned int * inIndices,IntDex inSizeOfIndices);
IntDex _MKT_loadAG(char * FP);