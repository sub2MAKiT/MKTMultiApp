#include <fileLoading/BM.h>
#include <../MKTDLL/MKTDLL.h>

#ifdef CURRENTP
#undef CURRENTP
#endif

#define CURRENTP _ren_sizeOfBM-1

IntDex _MKT_genBM(MKTbmdata * input)
{

    _ren_sizeOfBM++;
    _ren_BM = realloc(_ren_BM,_ren_sizeOfBM*sizeof(MKTbm));
    // input->w = 4608;
    // input->h = 8;
    VkDeviceSize imageSize = input->w * input->h;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    void* data;
    vkMapMemory(_device, stagingBufferMemory, 0, imageSize, 0, &data);
    char * tempO = data;
    char * tempI = (char*)input->pix;

    for(int i = 0; i < input->w*input->h; i++)
        tempO[i] = tempI[i];
        
    vkUnmapMemory(_device, stagingBufferMemory);

    free(input->pix);

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = input->w;
    imageInfo.extent.height = input->h;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // Optional

    VK_CHECK(vkCreateImage(_device, &imageInfo, NULL, &_ren_BM[CURRENTP].textureImage));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_device, _ren_BM[CURRENTP].textureImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VK_CHECK(vkAllocateMemory(_device, &allocInfo, NULL, &_ren_BM[CURRENTP].textureImageMemory));

    vkBindImageMemory(_device, _ren_BM[CURRENTP].textureImage, _ren_BM[CURRENTP].textureImageMemory, 0);

    transitionImageLayout(&_ren_BM[CURRENTP].textureImage, VK_FORMAT_R8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(stagingBuffer, &_ren_BM[CURRENTP].textureImage, input->w, input->h);
    transitionImageLayout(&_ren_BM[CURRENTP].textureImage, VK_FORMAT_R8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(_device, stagingBuffer, NULL);
    vkFreeMemory(_device, stagingBufferMemory, NULL);

    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = _ren_BM[CURRENTP].textureImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VK_CHECK(vkCreateImageView(_device, &viewInfo, NULL, &_ren_BM[CURRENTP].textureImageView));

    _ren_BM[CURRENTP].uniformBuffers = malloc(sizeof(VkBuffer)*MAX_FRAMES_IN_FLIGHT);
    _ren_BM[CURRENTP].uniformBuffersMemory = malloc(sizeof(VkDeviceMemory)*MAX_FRAMES_IN_FLIGHT);
    _ren_BM[CURRENTP].sizeOfUniformBuffers = MAX_FRAMES_IN_FLIGHT;

    VkDeviceSize bufferSize = sizeof(AGDescriptor);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &_ren_BM[CURRENTP].uniformBuffers[i], &_ren_BM[CURRENTP].uniformBuffersMemory[i]);
    }

    VkDescriptorPoolSize poolSizes[2];
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = MAX_FRAMES_IN_FLIGHT;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = MAX_FRAMES_IN_FLIGHT;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 2;
    poolInfo.pPoolSizes = poolSizes;
    poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

    VK_CHECK(vkCreateDescriptorPool(_device, &poolInfo, NULL, &_ren_BM[CURRENTP].descriptorPool));

    VkDescriptorSetLayout * layouts = malloc(MAX_FRAMES_IN_FLIGHT*sizeof(VkDescriptorSetLayout));
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT;i++)
        layouts[i] = _ren_materials[1].descriptorSetLayout;

    VkDescriptorSetAllocateInfo DesallocInfo = {};
    DesallocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    DesallocInfo.descriptorPool = _ren_BM[CURRENTP].descriptorPool;
    DesallocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
    DesallocInfo.pSetLayouts = layouts;
    
    
    _ren_BM[CURRENTP].descriptorSets = malloc(sizeof(VkDescriptorSet)*MAX_FRAMES_IN_FLIGHT);
    _ren_BM[CURRENTP].sizeOfDescriptorSets = MAX_FRAMES_IN_FLIGHT;

    VK_CHECK(vkAllocateDescriptorSets(_device, &DesallocInfo, _ren_BM[CURRENTP].descriptorSets));

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = _ren_BM[CURRENTP].uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(AGDescriptor);

        VkDescriptorImageInfo IimageInfo = {};
        IimageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        IimageInfo.imageView = _ren_BM[CURRENTP].textureImageView;
        IimageInfo.sampler = _textureSampler;

        VkWriteDescriptorSet * descriptorWrites = malloc(sizeof(VkWriteDescriptorSet)*2);

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].pNext = NULL;
        descriptorWrites[0].dstSet = _ren_BM[CURRENTP].descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].pNext = NULL;
        descriptorWrites[1].dstSet = _ren_BM[CURRENTP].descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &IimageInfo;

        vkUpdateDescriptorSets(_device, 2, descriptorWrites, 0, NULL);
        free(descriptorWrites);
    }

    AGDescriptor tempDescriptor = {(float)_WindowS.width/_WindowS.height,{0.0,0.0,0.0},{1.0,1.0,1.0,1.0},{2.0,2.0,2.0,1.0}};

    _VE_RUN_updateDescriptors(&tempDescriptor, sizeof(AGDescriptor), 1, CURRENTP);

    SAFEMALLOC(_ren_BM[CURRENTP]._dataPiC.vertices,(sizeof(PiCVertex)*input->sizeOfVertices));
    SAFEMALLOC(_ren_BM[CURRENTP]._dataPiC.indices,(sizeof(unsigned int)*input->sizeOfIndices));

    
    _ren_BM[CURRENTP]._dataPiC.sizeOfVertices = input->sizeOfVertices;
    _ren_BM[CURRENTP]._dataPiC.sizeOfIndices = input->sizeOfIndices;

    for(IntDex i = 0; i < input->sizeOfIndices;i++)
        _ren_BM[CURRENTP]._dataPiC.indices[i] = input->indices[i];

    createVertexBuffer(sizeof(PiCVertex)*_ren_BM[CURRENTP]._dataPiC.sizeOfVertices, input->vertices, &_ren_BM[CURRENTP].vertexBuffer,&_ren_BM[CURRENTP].vertexBufferMemory);
    createIndexBuffer(_ren_BM[CURRENTP]._dataPiC.sizeOfIndices, input->indices, &_ren_BM[CURRENTP].indexBuffer,&_ren_BM[CURRENTP].indexBufferMemory);

    free(input->vertices);
    free(input->indices);
    
    return CURRENTP;
}