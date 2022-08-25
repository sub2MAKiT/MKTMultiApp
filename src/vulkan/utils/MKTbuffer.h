#include <MKTAppEngine.h>

void createVertexBuffer(IntDex sizeOfBuffer, void * inData, VkBuffer * buffer,VkDeviceMemory * bufferMemory);
void createIndexBuffer(IntDex numOfIndices, void * inData,VkBuffer * buffer,VkDeviceMemory * bufferMemory);
void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer * buffer, VkDeviceMemory * bufferMemory);