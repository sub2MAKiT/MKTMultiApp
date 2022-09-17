#include <MKTAppEngine.h>
#include <utils/MKTbuffer.h>

VkCommandBuffer beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = _commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_graphicsQueue);

    vkFreeCommandBuffers(_device, _commandPool, 1, &commandBuffer);
}

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

unsigned int findMemoryType(unsigned int typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_chosenGPU, &memProperties);

    // <3
    for (unsigned int i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    MKTerror(102);
    return 0;
}

void createVertexBuffer(IntDex sizeOfBuffer, void * inData, VkBuffer * buffer,VkDeviceMemory * bufferMemory)
{
    VkDeviceSize bufferSize = sizeOfBuffer;

    printf("pointer two: %x\n",inData);
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    void* data;
    vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);

    char * tempO = data;
    char * tempI = inData;

    float * tempOO = data;
    float * tempII = inData;
    printf("pointer three: %x\n",tempII);

    printf("sizeIn: %d\n",sizeOfBuffer);

    // for(int i = 0; i < sizeOfBuffer; i++)
        // printf("%c",tempI[i]);

    // for(int i = 0; i < sizeOfBuffer; i++)
        // tempO[i] = tempI[i];
        // printf("\n\n\n");

    // for(int i = 0; i < sizeOfBuffer; i++)
        // printf("%c",tempO[i]);

    for(int i = 0; i < sizeOfBuffer/4; i++)
    {
        tempOO[i] = tempII[i];
        printf("%d.%f %f\n",i,tempOO[i],tempII[i]);
    }
    vkUnmapMemory(_device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);

    copyBuffer(stagingBuffer, *buffer, bufferSize);

    vkDestroyBuffer(_device, stagingBuffer, NULL);
    vkFreeMemory(_device, stagingBufferMemory, NULL);

    return;
}

void createIndexBuffer(IntDex numOfIndices, void * inData,VkBuffer * buffer,VkDeviceMemory * bufferMemory)
{
    VkDeviceSize bufferSize = sizeof(unsigned int) * numOfIndices;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);



    void* data;
    vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
    unsigned int * tempO = data;
    unsigned int * tempI = inData;
    for(int i = 0; i < numOfIndices; i++)
    {
        tempO[i] = tempI[i];
        printf("%d.indice: %d %d\n",i,tempI[i],tempO[i]);
    }

    vkUnmapMemory(_device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);

    copyBuffer(stagingBuffer, *buffer, bufferSize);

    vkDestroyBuffer(_device, stagingBuffer, NULL);
    vkFreeMemory(_device, stagingBufferMemory, NULL);

    return;
}

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer * buffer, VkDeviceMemory * bufferMemory) {
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK(vkCreateBuffer(_device, &bufferInfo, NULL, buffer));

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_device, *buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    VK_CHECK(vkAllocateMemory(_device, &allocInfo, NULL, bufferMemory));

    vkBindBufferMemory(_device, *buffer, *bufferMemory, 0);
}