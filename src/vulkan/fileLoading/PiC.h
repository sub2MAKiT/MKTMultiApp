#include <MKTAppEngine.h>
#include <structs.h>

IntDex _MKT_genPiC(MKTPiCdata input,PiCVertex * inVertices,IntDex inSizeOfVertices,unsigned int * inIndices,IntDex inSizeOfIndices);
MKTPiC _MKT_openPiC(char * FP);
IntDex _MKT_loadPiC(char * FP);