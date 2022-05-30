#pragma once

#include "types.h"

// default
#include <stdio.h>
#include <stdlib.h>
// boostrap
#include "vkb/VkBootstrap.h"

#define VK_CHECK(x) do{VkResult err = x;if (err){std::cout <<"Detected Vulkan error: " << err << std::endl;abort();}} while (0)