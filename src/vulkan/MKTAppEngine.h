#pragma once
#include "types.h"
#include <vector>
#include <cstdio>

#ifdef MAIN_MKT_APP_ENGINE_INCLUDE
#define MTKAEEXTERN
bool _isInitialized{false};
int _frameNumber {0};

VkExtent2D _windowExtent {1920, 1080};

struct SDL_Window* _window{NULL};
#else
#define MTKAEEXTERN extern
MTKAEEXTERN bool _isInitialized;
MTKAEEXTERN int _frameNumber;

MTKAEEXTERN VkExtent2D _windowExtent;

MTKAEEXTERN struct SDL_Window* _window;
#endif



MTKAEEXTERN void _MKTGE_init();

MTKAEEXTERN void _MKTGE_cleanup();

MTKAEEXTERN void _MKTGE_draw();

MTKAEEXTERN void _MKTGE_run();

MTKAEEXTERN VkInstance _instance;
MTKAEEXTERN VkDebugUtilsMessengerEXT _debug_messenger;
MTKAEEXTERN VkPhysicalDevice _chosenGPU;
MTKAEEXTERN VkDevice _device;
MTKAEEXTERN VkSurfaceKHR _surface;
MTKAEEXTERN VkSwapchainKHR _swapchain;
MTKAEEXTERN VkFormat _swapchainImageFormat;
MTKAEEXTERN std::vector<VkImage> _swapchainImages;
MTKAEEXTERN std::vector<VkImageView> _swapchainImageViews;
MTKAEEXTERN VkQueue _graphicsQueue;
MTKAEEXTERN uint32_t _graphicsQueueFamily;
MTKAEEXTERN VkCommandPool _commandPool;
MTKAEEXTERN VkCommandBuffer _mainCommandBuffer;
MTKAEEXTERN VkRenderPass _renderPass;
MTKAEEXTERN std::vector<VkFramebuffer> _framebuffers;
MTKAEEXTERN VkSemaphore _presentSemaphore, _renderSemaphore;
MTKAEEXTERN VkFence _renderFence;
MTKAEEXTERN VkPipelineLayout _trianglePipelineLayout;
MTKAEEXTERN VkPipeline _trianglePipeline;
MTKAEEXTERN void _MKTGE_init_vulkan();
MTKAEEXTERN void _MKTGE_init_swapchain();
MTKAEEXTERN void _MKTGE_init_commands();
MTKAEEXTERN void _MKTGE_init_default_renderpass();
MTKAEEXTERN void _MKTGE_init_framebuffers();
MTKAEEXTERN void _MKTGE_init_sync_structures();
MTKAEEXTERN void _MKTGE_init_pipelines();
MTKAEEXTERN bool _MKTGE_load_shader_module(const char* filePath, VkShaderModule* outShaderModule);
MTKAEEXTERN std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
MTKAEEXTERN VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
MTKAEEXTERN VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
MTKAEEXTERN VkViewport _viewport;
MTKAEEXTERN VkRect2D _scissor;
MTKAEEXTERN VkPipelineRasterizationStateCreateInfo _rasterizer;
MTKAEEXTERN VkPipelineColorBlendAttachmentState _colorBlendAttachment;
MTKAEEXTERN VkPipelineMultisampleStateCreateInfo _multisampling;
MTKAEEXTERN VkPipelineLayout _pipelineLayout;
MTKAEEXTERN VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);
MTKAEEXTERN VkCommandPoolCreateInfo command_pool_create_info(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags);
MTKAEEXTERN VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool pool, uint32_t count, VkCommandBufferLevel level);
MTKAEEXTERN VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
MTKAEEXTERN VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info();
MTKAEEXTERN VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info(VkPrimitiveTopology topology);
MTKAEEXTERN VkPipelineRasterizationStateCreateInfo rasterization_state_create_info(VkPolygonMode polygonMode);
MTKAEEXTERN VkPipelineMultisampleStateCreateInfo multisampling_state_create_info();
MTKAEEXTERN VkPipelineColorBlendAttachmentState color_blend_attachment_state();
MTKAEEXTERN VkPipelineLayoutCreateInfo pipeline_layout_create_info();