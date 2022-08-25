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

    for(IntDex i = 0; i < inSizeOfIndices;i++)
        _REN_AG[CURRENT].indices[i] = inIndices[i];

    free(inVertices);
    free(inIndices);

    createVertexBuffer(sizeof(AGVertex)*_REN_AG[CURRENT].sizeOfVertices, _REN_AG[CURRENT].vertices, &_REN_AG[CURRENT].vertexBuffer,&_REN_AG[CURRENT].vertexBufferMemory);
    createIndexBuffer(_REN_AG[CURRENT].sizeOfIndices, _REN_AG[CURRENT].indices, &_REN_AG[CURRENT].indexBuffer,&_REN_AG[CURRENT].indexBufferMemory);

    // free(triangle);
    
    return CURRENT;
}