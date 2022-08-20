#include "MKTbuffer.h"
#include "../DEBUG.h"

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_chosenGPU, &memProperties);

    // this loop is not my code, but I can not express how much I love it.
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    //error
    return 0;
}

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    VkCommandPool cmdPool = get_current_frame()._commandPool;
    allocInfo.commandPool = cmdPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_graphicsQueue);

    vkFreeCommandBuffers(_device, cmdPool, 1, &commandBuffer);
}

void createVertexBuffer(size_t sizeOfBuffer, void * _vertices , VkBuffer * _vertexBuffer, VkDeviceMemory * _vertexBufferMemory)
{
    VkDeviceSize bufferSize = sizeOfBuffer;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    DEBUG("createVertexBuffer->before creating the staging buffer");
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);
    DEBUG("createVertexBuffer->after creating the staging buffer");

    // createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _vertexBuffer, _vertexBufferMemory);

    void* data;
    DEBUG("createVertexBuffer->started memory mapping");
    VK_CHECK(vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data));
    DEBUG("createVertexBuffer->memory mapped");
    memcpy(data, _vertices, sizeOfBuffer);
    DEBUG("createVertexBuffer->memory coppied");
    vkUnmapMemory(_device, stagingBufferMemory);
    DEBUG("createVertexBuffer->memory unmapped");

    DEBUG("createVertexBuffer->before creating the final buffer");
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexBufferMemory);
    DEBUG("createVertexBuffer->after creating the final buffer");

    copyBuffer(stagingBuffer, *_vertexBuffer, bufferSize);
    DEBUG("createVertexBuffer->buffer coppied");

    vkDestroyBuffer(_device, stagingBuffer, NULL);
    vkFreeMemory(_device, stagingBufferMemory, NULL);

    return;
}


// void createPiCBuffer(MKTAPiC * _vertices, size_t _verN, VkBuffer * _vertexBuffer, VkDeviceMemory * _vertexBufferMemory)
// {
//     VkBufferCreateInfo bufferInfo = {};
//     bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//     bufferInfo.size = sizeof(MKTAPiC) * _verN;
//     bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
//     bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

//     vkCreateBuffer(_device, &bufferInfo, NULL, _vertexBuffer);

//     VkMemoryRequirements memRequirements;
//     vkGetBufferMemoryRequirements(_device, *_vertexBuffer, &memRequirements);

//     VkMemoryAllocateInfo allocInfo = {};
//     allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//     allocInfo.allocationSize = memRequirements.size;
//     allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

//     VK_CHECK(vkAllocateMemory(_device, &allocInfo, NULL, _vertexBufferMemory));

//     VK_CHECK(vkBindBufferMemory(_device, *_vertexBuffer, *_vertexBufferMemory, 0));

//     void* data;
//     vkMapMemory(_device, *_vertexBufferMemory, 0, bufferInfo.size, 0, &data);
//     memcpy(data, _vertices, (size_t)bufferInfo.size);
//     vkUnmapMemory(_device, *_vertexBufferMemory);

//     return;
// }

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer * buffer, VkDeviceMemory * bufferMemory) {
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK(vkCreateBuffer(_device, &bufferInfo, NULL, buffer));

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_device, *buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    VK_CHECK(vkAllocateMemory(_device, &allocInfo, NULL, bufferMemory));

    vkBindBufferMemory(_device, *buffer, *bufferMemory, 0);

    return;
}