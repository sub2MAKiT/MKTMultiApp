#include <fileLoading/AG.h>

#define CURRENT _ren_sizeOfAG-1

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
    FILE * MKTFILE = fopen(FP,"rb");
    float * list;
    fseek(MKTFILE, 0L, SEEK_END);
    long sizeOfFile = ftell(MKTFILE);
    rewind(MKTFILE);
    SAFEMALLOC(list,sizeOfFile);
    fread( list,1, sizeOfFile, MKTFILE );
    fclose(MKTFILE);

    MKTag returnAG;
    returnAG.sizeOfVertices = *(unsigned long long int*)(list);
    SAFEMALLOC(returnAG.vertices,returnAG.sizeOfVertices*sizeof(float)*6);
    for(int i = 0; i < returnAG.sizeOfVertices; i++) // xyrgba
    {
        returnAG.vertices[i].pos.x = list[2+i*6];
        returnAG.vertices[i].pos.y = list[3+i*6];
        returnAG.vertices[i].colour.r = list[4+i*6];
        returnAG.vertices[i].colour.g = list[5+i*6];
        returnAG.vertices[i].colour.b = list[6+i*6];
        returnAG.vertices[i].colour.a = list[7+i*6];
    }

    returnAG.sizeOfIndices = ((sizeOfFile-returnAG.sizeOfVertices*sizeof(float)*6)-8)/4;

    SAFEMALLOC(returnAG.indices,returnAG.sizeOfIndices*sizeof(unsigned int))

    for(int i = 0; i < returnAG.sizeOfIndices; i++)
        returnAG.indices[i] = *(unsigned int*)&list[i+returnAG.sizeOfVertices*6+2];

    return returnAG;
}

IntDex _MKT_loadAG(char * FP)
{
    MKTag loaded = _MKT_openAG(FP);

    return _MKT_genAG(loaded.vertices,loaded.sizeOfVertices,loaded.indices,loaded.sizeOfIndices);
}