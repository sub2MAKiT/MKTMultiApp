#include <fileLoading/QDR.h>

#define CURRENT _ren_sizeOfQDR-1
#define defaultMatrixValue  2.0,0.0,0.0,0.0,\
                            0.0,2.0,0.0,0.0,\
                            0.0,0.0,2.0,0.0,\
                            0.0,0.0,0.0,1.0

IntDex _MKT_genQDR(MKTQDRdata * input)
{
    _ren_sizeOfQDR++;
    _ren_QDR = realloc(_ren_QDR,_ren_sizeOfQDR*sizeof(MKTqdr));

    SAFEMALLOC(_ren_QDR[CURRENT]._dataQDR.vertices,(sizeof(QDRVertex)*input->sizeOfVertices));
    SAFEMALLOC(_ren_QDR[CURRENT]._dataQDR.indices,(sizeof(unsigned int)*input->sizeOfIndices));

    
    _ren_QDR[CURRENT]._dataQDR.sizeOfVertices = input->sizeOfVertices;
    _ren_QDR[CURRENT]._dataQDR.sizeOfIndices = input->sizeOfIndices;

    for(IntDex i = 0; i < input->sizeOfVertices;i++)
        _ren_QDR[CURRENT]._dataQDR.vertices[i] = input->vertices[i];
    DEBUG("II init:VEvariables II test2");

    // printf("test: %d %d\n",input->sizeOfIndices,input->indices[0]);

    for(IntDex i = 0; i < input->sizeOfIndices;i++)
        _ren_QDR[CURRENT]._dataQDR.indices[i] = input->indices[i];

// _ren_QDR[CURRENT]._dataQDR.indices[0] = 0;

    DEBUG("II init:VEvariables II test");
    free(input->vertices);
    free(input->indices);

    createVertexBuffer(sizeof(AGVertex)*_ren_QDR[CURRENT]._dataQDR.sizeOfVertices, _ren_QDR[CURRENT]._dataQDR.vertices, &_ren_QDR[CURRENT].vertexBuffer,&_ren_QDR[CURRENT].vertexBufferMemory);
    createIndexBuffer(_ren_QDR[CURRENT]._dataQDR.sizeOfIndices, _ren_QDR[CURRENT]._dataQDR.indices, &_ren_QDR[CURRENT].indexBuffer,&_ren_QDR[CURRENT].indexBufferMemory);

    VkDeviceSize bufferSize = sizeof(AGDescriptor);

    _ren_QDR[CURRENT].uniformBuffers = malloc(sizeof(VkBuffer)*MAX_FRAMES_IN_FLIGHT);
    _ren_QDR[CURRENT].uniformBuffersMemory = malloc(sizeof(VkDeviceMemory)*MAX_FRAMES_IN_FLIGHT);
    _ren_QDR[CURRENT].sizeOfUniformBuffers = MAX_FRAMES_IN_FLIGHT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &_ren_QDR[CURRENT].uniformBuffers[i], &_ren_QDR[CURRENT].uniformBuffersMemory[i]);
    }

    VkDescriptorPoolSize poolSize = {};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

    VK_CHECK(vkCreateDescriptorPool(_device, &poolInfo, NULL, &_ren_QDR[CURRENT].descriptorPool));

    VkDescriptorSetLayout * layouts = malloc(MAX_FRAMES_IN_FLIGHT*sizeof(VkDescriptorSetLayout));
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT;i++)
        layouts[i] = _ren_materials[0].descriptorSetLayout;

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = _ren_QDR[CURRENT].descriptorPool;
    allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts;
    
    
    _ren_QDR[CURRENT].descriptorSets = malloc(sizeof(VkDescriptorSet)*MAX_FRAMES_IN_FLIGHT);
    _ren_QDR[CURRENT].sizeOfDescriptorSets = MAX_FRAMES_IN_FLIGHT;

    VK_CHECK(vkAllocateDescriptorSets(_device, &allocInfo, _ren_QDR[CURRENT].descriptorSets));

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = _ren_QDR[CURRENT].uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(AGDescriptor);

        VkWriteDescriptorSet descriptorWrite = {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = _ren_QDR[CURRENT].descriptorSets[i];
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

    // _VE_RUN_updateDescriptors(&tempDescriptor, sizeof(AGDescriptor), 3, CURRENT);

    return CURRENT;
}