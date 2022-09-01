#include <fileLoading/AG.h>

#define CURRENT _ren_sizeOfAG-1

IntDex _MKT_genAG(AGVertex * inVertices,IntDex inSizeOfVertices,unsigned int * inIndices,IntDex inSizeOfIndices)
{
    _ren_sizeOfAG++;
    _ren_AG = realloc(_ren_AG,_ren_sizeOfAG);

    SAFEMALLOC(_ren_AG[CURRENT].vertices,(sizeof(AGVertex)*inSizeOfVertices));
    SAFEMALLOC(_ren_AG[CURRENT].indices,(sizeof(unsigned int)*inSizeOfIndices));
    
    _ren_AG[CURRENT].sizeOfVertices = inSizeOfVertices;
    _ren_AG[CURRENT].sizeOfIndices = inSizeOfIndices;

    for(IntDex i = 0; i < inSizeOfVertices;i++)
        _ren_AG[CURRENT].vertices[i] = inVertices[i];
        
    for(IntDex i = 0; i < inSizeOfIndices;i++)
    {
        _ren_AG[CURRENT].indices[i] = inIndices[i];
    }

    free(inVertices);
    free(inIndices);

    createVertexBuffer(sizeof(AGVertex)*_ren_AG[CURRENT].sizeOfVertices, _ren_AG[CURRENT].vertices, &_ren_AG[CURRENT].vertexBuffer,&_ren_AG[CURRENT].vertexBufferMemory);
    createIndexBuffer(_ren_AG[CURRENT].sizeOfIndices, _ren_AG[CURRENT].indices, &_ren_AG[CURRENT].indexBuffer,&_ren_AG[CURRENT].indexBufferMemory);



    // free(triangle);
    
    return CURRENT;
}