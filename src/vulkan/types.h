#pragma once
#include <vulkan/vulkan.h>
#include <VulkanMA/vk_mem_alloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MTDOUBLE double

struct MKTvec3 {
    MTDOUBLE value[3];
};

typedef struct MKTvec3 MKTV3;

struct AllocatedBuffer {
    VkBuffer _buffer;
    VmaAllocation _allocation;
};

typedef struct AllocatedBuffer allocatedBuffer;