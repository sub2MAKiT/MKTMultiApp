#pragma once
#include <vulkan/vulkan.h>
#include <VulkanMA/vk_mem_alloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MKTDOUBLE float

struct MKTvec3 {
    MKTDOUBLE value[3];
};

typedef struct MKTvec3 MKTV3;

struct AllocatedBuffer {
    VkBuffer _buffer;
    VmaAllocation _allocation;
};

typedef struct AllocatedBuffer allocatedBuffer;

struct MKTVID {

	const VkVertexInputBindingDescription * bindings;
	const VkVertexInputAttributeDescription * attributes;
    size_t sizeOfArrayB;
    size_t sizeOfArrayA;

	VkPipelineVertexInputStateCreateFlags flags = 1;
};

typedef struct MKTVID VertexInputDescription;