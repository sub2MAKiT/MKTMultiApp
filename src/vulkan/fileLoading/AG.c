#include <fileLoading/AG.h>

#define CURRENT _REN_sizeOfAG-1

IntDex _MKT_genAG(AGVertex * inVertices,IntDex inSizeOfVertices,unsigned int * inIndices,IntDex inSizeOfIndices)
{
    _REN_sizeOfAG++;
    _REN_AG = realloc(_REN_AG,_REN_sizeOfAG);

    _REN_AG[CURRENT].vertices = malloc(sizeof(AGVertex)*inSizeOfVertices);
    _REN_AG[CURRENT].indices = malloc(sizeof(unsigned int)*inSizeOfIndices);
    
    _REN_AG[CURRENT].sizeOfVertices = inSizeOfVertices;
    _REN_AG[CURRENT].sizeOfIndices = inSizeOfIndices;

    for(IntDex i = 0; i < inSizeOfVertices;i++)
        _REN_AG[CURRENT].vertices[i] = inVertices[i];

    // DEBUG("I TEMP DEBUG I");
    // printf("size: %d\n",inSizeOfIndices);
    for(IntDex i = 0; i < inSizeOfIndices;i++)
    {
        // printf("%d.test: %d %d\n",i,inIndices[i]);
        _REN_AG[CURRENT].indices[i] = inIndices[i];
    // DEBUG("I TEMP DEBUG1 I");
    }
    DEBUG("I TEMP DEBUG I");

    free(inVertices);
    free(inIndices);
    DEBUG("I TEMP DEBUG I");

    createVertexBuffer(sizeof(AGVertex)*_REN_AG[CURRENT].sizeOfVertices, _REN_AG[CURRENT].vertices, &_REN_AG[CURRENT].vertexBuffer,&_REN_AG[CURRENT].vertexBufferMemory);
    createIndexBuffer(_REN_AG[CURRENT].sizeOfIndices, _REN_AG[CURRENT].indices, &_REN_AG[CURRENT].indexBuffer,&_REN_AG[CURRENT].indexBufferMemory);
    DEBUG("I TEMP DEBUG I");



    // free(triangle);
    
    return CURRENT;
}