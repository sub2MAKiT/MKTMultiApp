#pragma once
#include "MKTAppEngine.h"
#include "MKTMesh.h"
//_MKTENGINEINCLUDEGUARD_

void createVertexBuffer(size_t sizeOfBuffer, void * _vertices, VkBuffer * _vertexBuffer, VkDeviceMemory * _vertexBufferMemory);
void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer * buffer, VkDeviceMemory * bufferMemory);