#include <fileLoading/AG.h>
#include <../MKTDLL/MKTDLL.h>

#define CURRENT _ren_sizeOfAG-1

typedef struct posXYtemp {
    float x;
    float y;
} posxytemp;

typedef struct colourtemp {
    float r;
    float g;
    float b;
    float a;
} colourRGBAtemp;

typedef struct verticestemp {
posxytemp pos;
colourRGBAtemp colour;
} verticetemp;

typedef struct MKTAGtemp {
    unsigned long long int sizeOfVertices;
    unsigned long long int sizeOfIndices;
    verticetemp * vertices;
    unsigned int * indices;
} MKTagFtemp;

IntDex _MKT_genAG(AGVertex * inVertices,IntDex inSizeOfVertices,unsigned int * inIndices,IntDex inSizeOfIndices)
{
    _ren_sizeOfAG++;
    _ren_AG = realloc(_ren_AG,_ren_sizeOfAG*sizeof(MKTag));

    SAFEMALLOC(_ren_AG[CURRENT].vertices,(sizeof(AGVertex)*inSizeOfVertices));
    SAFEMALLOC(_ren_AG[CURRENT].indices,(sizeof(unsigned int)*inSizeOfIndices));

    
    _ren_AG[CURRENT].sizeOfVertices = inSizeOfVertices;
    _ren_AG[CURRENT].sizeOfIndices = inSizeOfIndices;

    for(IntDex i = 0; i < inSizeOfVertices;i++)
        _ren_AG[CURRENT].vertices[i] = inVertices[i];

    for(IntDex i = 0; i < inSizeOfIndices;i++)
        _ren_AG[CURRENT].indices[i] = inIndices[i];

    free(inVertices);
    free(inIndices);

    createVertexBuffer(sizeof(AGVertex)*_ren_AG[CURRENT].sizeOfVertices, _ren_AG[CURRENT].vertices, &_ren_AG[CURRENT].vertexBuffer,&_ren_AG[CURRENT].vertexBufferMemory);
    createIndexBuffer(_ren_AG[CURRENT].sizeOfIndices, _ren_AG[CURRENT].indices, &_ren_AG[CURRENT].indexBuffer,&_ren_AG[CURRENT].indexBufferMemory);

    return CURRENT;
}

MKTag _MKT_openAG(char * FP)
{
    MKTagFtemp tempAGData;
    MKTInfo * tempData;
    MKTag returnAG;
    unsigned long long int tempSize = 0;
    for(int i = 0; i < _MKT_sizeOfFileModules;i++)
        if(MKTstrcmp(MKTdissectFileType(&tempSize, FP), _MKT_fileModules[i].FileName))
            tempData = (*_MKT_fileModules[i].load)(FP);

    tempAGData = *(MKTagFtemp*)tempData->data;

    if(tempData->ID)
        MKTerror(tempData->ID);

    if(tempData->type != 1)
        MKTerror(200);

    free(tempData);

    returnAG.vertices = (AGVertex*)tempAGData.vertices;
    returnAG.sizeOfVertices = tempAGData.sizeOfVertices;
    returnAG.sizeOfIndices = tempAGData.sizeOfIndices;
    returnAG.indices = tempAGData.indices;

    return returnAG;
}

IntDex _MKT_loadAG(char * FP)
{
    MKTag loaded = _MKT_openAG(FP);

    return _MKT_genAG(loaded.vertices,loaded.sizeOfVertices,loaded.indices,loaded.sizeOfIndices);
}