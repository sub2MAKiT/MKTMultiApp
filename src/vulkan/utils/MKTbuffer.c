#include <MKTAppEngine.h>
#include <utils/MKTbuffer.h>

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

void createVertexBuffer()
{

    return;
}

void createIndexBuffer()
{
    return;
}

void createBuffer(IntDex sizeOfBuffer, void * inData, VkBuffer * buffer,VkDeviceMemory * bufferMemory)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeOfBuffer;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VK_CHECK(vkCreateBuffer(_device, &bufferInfo, NULL, buffer));

    
    // DELQUEVARIABLES
    // VkDevice a;VkBuffer b;const VkAllocationCallbacks* c;
    // DELQUEVARIABLESDOT(vkDestroyBuffer)
    // input->a,input->b,input->c
    // DELQUEVARIABLESVALUE
    // _device,*buffer,NULL
    // COMMITDELQUE(vkDestroyBuffer)

    

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_device, *buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VK_CHECK(vkAllocateMemory(_device, &allocInfo, NULL, bufferMemory));

    vkBindBufferMemory(_device, *buffer, *bufferMemory, 0);

    // DELQUEVARIABLES
    // VkDevice a;VkDeviceMemory b;const VkAllocationCallbacks* c;
    // DELQUEVARIABLESDOT(vkFreeMemory)
    // input->a,input->b,input->c
    // DELQUEVARIABLESVALUE
    // _device,*bufferMemory,NULL
    // COMMITDELQUE(vkFreeMemory)


    void* data;
    vkMapMemory(_device, *bufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, inData, bufferInfo.size);
    vkUnmapMemory(_device, *bufferMemory);

    DEBUG("TEST");
    return;
}