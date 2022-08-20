#pragma once
#include <types.h>
// #include <OutputTypes/MKTMesh.h>
//_MKTENGINEINCLUDEGUARD_

unsigned int findMemoryType(unsigned int typeFilter, VkMemoryPropertyFlags properties)
void createVertexBuffer(size_t sizeOfBuffer, void * _vertices, VkBuffer * _vertexBuffer, VkDeviceMemory * _vertexBufferMemory);
void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer * buffer, VkDeviceMemory * bufferMemory);