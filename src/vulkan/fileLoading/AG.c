#include <fileLoading/AG.h>
#include <../MKTDLL/MKTDLL.h>

#define CURRENT _ren_sizeOfAG-1
#define defaultMatrixValue  2.0,0.0,0.0,0.0,\
                            0.0,2.0,0.0,0.0,\
                            0.0,0.0,2.0,0.0,\
                            0.0,0.0,0.0,1.0



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

    VkDeviceSize bufferSize = sizeof(AGDescriptor);

    _ren_AG[CURRENT].uniformBuffers = malloc(sizeof(VkBuffer)*MAX_FRAMES_IN_FLIGHT);
    _ren_AG[CURRENT].uniformBuffersMemory = malloc(sizeof(VkDeviceMemory)*MAX_FRAMES_IN_FLIGHT);
    _ren_AG[CURRENT].sizeOfUniformBuffers = MAX_FRAMES_IN_FLIGHT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &_ren_AG[CURRENT].uniformBuffers[i], &_ren_AG[CURRENT].uniformBuffersMemory[i]);
    }

    VkDescriptorPoolSize poolSize = {};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

    VK_CHECK(vkCreateDescriptorPool(_device, &poolInfo, NULL, &_ren_AG[CURRENT].descriptorPool));

    VkDescriptorSetLayout * layouts = malloc(MAX_FRAMES_IN_FLIGHT*sizeof(VkDescriptorSetLayout));
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT;i++)
        layouts[i] = _ren_materials[0].descriptorSetLayout;

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = _ren_AG[CURRENT].descriptorPool;
    allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts;
    
    
    _ren_AG[CURRENT].descriptorSets = malloc(sizeof(VkDescriptorSet)*MAX_FRAMES_IN_FLIGHT);
    _ren_AG[CURRENT].sizeOfDescriptorSets = MAX_FRAMES_IN_FLIGHT;

    VK_CHECK(vkAllocateDescriptorSets(_device, &allocInfo, _ren_AG[CURRENT].descriptorSets));

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = _ren_AG[CURRENT].uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(AGDescriptor);

        VkWriteDescriptorSet descriptorWrite = {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = _ren_AG[CURRENT].descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;

        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = NULL; // Optional
        descriptorWrite.pTexelBufferView = NULL; // Optional

        vkUpdateDescriptorSets(_device, 1, &descriptorWrite, 0, NULL);
    }


                                    //              r/?/?/?/mov.x/mov.y/mov.y/?
    AGDescriptor tempDescriptor = {(float)_WindowS.width/_WindowS.height,{0.0,0.0,0.0},{1.0,0.0,1.0,1.0},{2.0,2.0,2.0,1.0}};//,{defaultMatrixValue}};

    _VE_RUN_updateDescriptors(&tempDescriptor, sizeof(AGDescriptor), 0, CURRENT);

    return CURRENT;
}

MKTag _MKT_openAG(char * FP)
{
    MKTAGdata tempAGData;
    MKTInfo * tempData;
    MKTag returnAG;
    unsigned long long int tempSize = 0;
    for(int i = 0; i < _MKT_sizeOfFileModules;i++)
        if(MKTstrcmp(MKTdissectFileType(&tempSize, FP), _MKT_fileModules[i].FileName))
            tempData = (*_MKT_fileModules[i].load)(FP);

    tempAGData = *(MKTAGdata*)tempData->data;

    if(tempData->ID)
        MKTerror(tempData->ID);

    if(tempData->type != 1)
        MKTerror(200);

    free(tempData);

    returnAG.vertices = (AGVertex*)tempAGData.vertices;
    returnAG.sizeOfVertices = tempAGData.sizeOfVertices;
    returnAG.sizeOfIndices = tempAGData.sizeOfIndices;
    returnAG.indices = tempAGData.indices;

    // free(tempData->data);

    return returnAG;
}

IntDex _MKT_loadAG(char * FP)
{
    MKTag loaded = _MKT_openAG(FP);
    return _MKT_genAG(loaded.vertices,loaded.sizeOfVertices,loaded.indices,loaded.sizeOfIndices);
}