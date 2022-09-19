#include <fileLoading/PiC.h>

#define CURRENTP _ren_sizeOfPiC-1

void copyBufferToImage(VkBuffer buffer, VkImage * image, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = (VkOffset3D){0, 0, 0};
    region.imageExtent = (VkExtent3D){
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        *image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    endSingleTimeCommands(commandBuffer);
}

void transitionImageLayout(VkImage * image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = *image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0; // TODO
    barrier.dstAccessMask = 0; // TODO

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        MKTerror(103);
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, NULL,
        0, NULL,
        1, &barrier
    );

    endSingleTimeCommands(commandBuffer);
}

IntDex _MKT_genPiC(MKTPiCdata input,PiCVertex * inVertices,IntDex inSizeOfVertices,unsigned int * inIndices,IntDex inSizeOfIndices) 
{
    _ren_sizeOfPiC++;
    _ren_PiC = realloc(_ren_PiC,_ren_sizeOfPiC*sizeof(MKTPiC));
    VkDeviceSize imageSize = input.w * input.h * 4;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    void* data;
    vkMapMemory(_device, stagingBufferMemory, 0, imageSize, 0, &data);
    char * tempO = data;
    char * tempI = (char*)input.pix;

    for(int i = 0; i < input.w*input.h*4; i++)
        tempO[i] = tempI[i];
    vkUnmapMemory(_device, stagingBufferMemory);

    free(input.pix);

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = input.w;
    imageInfo.extent.height = input.h;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // Optional

    VK_CHECK(vkCreateImage(_device, &imageInfo, NULL, &_ren_PiC[CURRENTP].textureImage));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_device, _ren_PiC[CURRENTP].textureImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VK_CHECK(vkAllocateMemory(_device, &allocInfo, NULL, &_ren_PiC[CURRENTP].textureImageMemory));

    vkBindImageMemory(_device, _ren_PiC[CURRENTP].textureImage, _ren_PiC[CURRENTP].textureImageMemory, 0);

    transitionImageLayout(&_ren_PiC[CURRENTP].textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(stagingBuffer, &_ren_PiC[CURRENTP].textureImage, input.w, input.h);
    transitionImageLayout(&_ren_PiC[CURRENTP].textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(_device, stagingBuffer, NULL);
    vkFreeMemory(_device, stagingBufferMemory, NULL);

    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = _ren_PiC[CURRENTP].textureImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VK_CHECK(vkCreateImageView(_device, &viewInfo, NULL, &_ren_PiC[CURRENTP].textureImageView));

    _ren_PiC[CURRENTP].uniformBuffers = malloc(sizeof(VkBuffer)*MAX_FRAMES_IN_FLIGHT);
    _ren_PiC[CURRENTP].uniformBuffersMemory = malloc(sizeof(VkDeviceMemory)*MAX_FRAMES_IN_FLIGHT);
    _ren_PiC[CURRENTP].sizeOfUniformBuffers = MAX_FRAMES_IN_FLIGHT;

    VkDeviceSize bufferSize = sizeof(AGDescriptor);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &_ren_PiC[CURRENTP].uniformBuffers[i], &_ren_PiC[CURRENTP].uniformBuffersMemory[i]);
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

    VK_CHECK(vkCreateDescriptorPool(_device, &poolInfo, NULL, &_ren_PiC[CURRENTP].descriptorPool));

    VkDescriptorSetLayout * layouts = malloc(MAX_FRAMES_IN_FLIGHT*sizeof(VkDescriptorSetLayout));
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT;i++)
        layouts[i] = _ren_materials[1].descriptorSetLayout;

    VkDescriptorSetAllocateInfo DesallocInfo = {};
    DesallocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    DesallocInfo.descriptorPool = _ren_PiC[CURRENTP].descriptorPool;
    DesallocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
    DesallocInfo.pSetLayouts = layouts;
    
    
    _ren_PiC[CURRENTP].descriptorSets = malloc(sizeof(VkDescriptorSet)*MAX_FRAMES_IN_FLIGHT);
    _ren_PiC[CURRENTP].sizeOfDescriptorSets = MAX_FRAMES_IN_FLIGHT;

    VK_CHECK(vkAllocateDescriptorSets(_device, &DesallocInfo, _ren_PiC[CURRENTP].descriptorSets));

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = _ren_PiC[CURRENTP].uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(AGDescriptor);

        VkDescriptorImageInfo IimageInfo = {};
        IimageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        IimageInfo.imageView = _ren_PiC[CURRENTP].textureImageView;
        IimageInfo.sampler = _textureSampler;

        VkWriteDescriptorSet * descriptorWrites = malloc(sizeof(VkWriteDescriptorSet)*2);

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].pNext = NULL;
        descriptorWrites[0].dstSet = _ren_PiC[CURRENTP].descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].pNext = NULL;
        descriptorWrites[1].dstSet = _ren_PiC[CURRENTP].descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &IimageInfo;

        vkUpdateDescriptorSets(_device, 2, descriptorWrites, 0, NULL);
        free(descriptorWrites);
    }

    AGDescriptor tempDescriptor = {(float)_WindowS.width/_WindowS.height,{0.0,0.0,0.0},{1.0,0.0,1.0,1.0},{2.0,2.0,2.0,1.0}};

    _VE_RUN_updateDescriptors(&tempDescriptor, sizeof(AGDescriptor), 1, CURRENTP);

    SAFEMALLOC(_ren_PiC[CURRENTP].vertices,(sizeof(PiCVertex)*inSizeOfVertices));
    SAFEMALLOC(_ren_PiC[CURRENTP].indices,(sizeof(unsigned int)*inSizeOfIndices));

    
    _ren_PiC[CURRENTP].sizeOfVertices = inSizeOfVertices;
    _ren_PiC[CURRENTP].sizeOfIndices = inSizeOfIndices;

    for(IntDex i = 0; i < inSizeOfIndices;i++)
        _ren_PiC[CURRENTP].indices[i] = inIndices[i];

    createVertexBuffer(sizeof(PiCVertex)*_ren_PiC[CURRENTP].sizeOfVertices, inVertices, &_ren_PiC[CURRENTP].vertexBuffer,&_ren_PiC[CURRENTP].vertexBufferMemory);
    createIndexBuffer(_ren_PiC[CURRENTP].sizeOfIndices, inIndices, &_ren_PiC[CURRENTP].indexBuffer,&_ren_PiC[CURRENTP].indexBufferMemory);

    free(inVertices);
    free(inIndices);
    
    return CURRENTP;
}

MKTPiC _MKT_openPiC(char * FP)
{
    MKTPiC triangle;
    SAFEMALLOC(triangle.vertices,sizeof(PiCVertex)*4);
    triangle.vertices[0] = (PiCVertex){{1.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}};
    triangle.vertices[1] = (PiCVertex){{1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}};
    triangle.vertices[2] = (PiCVertex){{-1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};
    triangle.vertices[3] = (PiCVertex){{-1.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}};

    SAFEMALLOC(triangle.indices,sizeof(unsigned int)*6);
    triangle.sizeOfIndices = 6;
    triangle.sizeOfVertices = 4;

    triangle.indices[0] = 0;
    triangle.indices[1] = 1;
    triangle.indices[2] = 2;
    triangle.indices[3] = 0;
    triangle.indices[4] = 2;
    triangle.indices[5] = 3;

    FILE * MKTFILE = fopen(FP,"rb");
    // FILE * MKTFILE = fopen("../logo/icon.MKTI","rb");

    fseek(MKTFILE, 0L, SEEK_END);
    unsigned long sizeOfFile = ftell(MKTFILE);
    rewind(MKTFILE);
    char * pixelArray = malloc(sizeOfFile);
    fread( pixelArray,1, sizeOfFile, MKTFILE );
    fclose(MKTFILE);

    unsigned long long int tempWidth = *(unsigned long long int *)pixelArray;
    unsigned long long int tempHeight = *(unsigned long long int *)(pixelArray+8);

    for(int i = 0; i < sizeOfFile-16; i++)
        pixelArray[i] = pixelArray[i+16];
    
    MKTPiCdata tempInputLoaded = {tempWidth,tempHeight,(MKTrgbaP*)(pixelArray)};

    triangle._dataPiC = tempInputLoaded;

    return triangle;
}

IntDex _MKT_loadPiC(char * FP)
{
    MKTPiC loaded = _MKT_openPiC(FP);

    return _MKT_genPiC(loaded._dataPiC,loaded.vertices,loaded.sizeOfVertices,loaded.indices,loaded.sizeOfIndices);
}