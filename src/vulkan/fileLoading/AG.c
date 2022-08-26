#include <fileLoading/AG.h>

#define CURRENT _ren_sizeOfAG-1

IntDex _MKT_genAG(AGVertex * inVertices,IntDex inSizeOfVertices,unsigned int * inIndices,IntDex inSizeOfIndices)
{
    _ren_sizeOfAG++;
    _ren_AG = realloc(_ren_AG,_ren_sizeOfAG);

    _ren_AG[CURRENT].vertices = malloc(sizeof(AGVertex)*inSizeOfVertices);
    printf("inSizeV: %d\n",inSizeOfVertices);
    printf("test %x\n",_ren_AG[CURRENT].indices);
    _ren_AG[CURRENT].indices = malloc(sizeof(unsigned int)*inSizeOfIndices);
    printf("test1 %x\n",_ren_AG[CURRENT].indices);
    
    _ren_AG[CURRENT].sizeOfVertices = inSizeOfVertices;
    printf("test2 %x\n",_ren_AG[CURRENT].indices);
    _ren_AG[CURRENT].sizeOfIndices = inSizeOfIndices;
    printf("test3 %x\n",_ren_AG[CURRENT].indices);

    for(IntDex i = 0; i < inSizeOfVertices;i++)
    {
        printf("%d.test4 %x\n",i,_ren_AG[CURRENT].indices);
        _ren_AG[CURRENT].vertices[i] = inVertices[i];
        printf("%d.test5 %x\n",i,_ren_AG[CURRENT].indices);
    }
    printf("test6 %x\n",_ren_AG[CURRENT].indices);
    printf("test7 %x\n",_ren_AG[CURRENT].indices);
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