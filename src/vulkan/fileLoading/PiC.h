#include <MKTAppEngine.h>
#include <structs.h>

typedef struct MKTRGBAP {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} MKTrgbaP;

typedef struct MKTPICDATA {
    IntDex w;
    IntDex h;
    MKTrgbaP * pix;
} MKTPiCdata;

void createTextureImage(MKTPiCdata input,PiCVertex * inVertices,IntDex inSizeOfVertices,unsigned int * inIndices,IntDex inSizeOfIndices);