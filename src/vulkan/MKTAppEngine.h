#pragma once
#include "types.h"
#include "MKTArrayGraphics.h"
#include "MKTMesh.h"
#include <vector>
#include <cstdio>
#include <deque>
#include <functional>

struct DeletionQueue {
	std::deque<std::function<void()>> deletors;

	void push_function(std::function<void()>&& function) {
		deletors.push_back(function);
	}

	void flush() {
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
			(*it)();
		}

		deletors.clear();
	}
};

typedef struct DeletionQueue MainDeletionQueue;

#ifdef MAIN_MKT_APP_ENGINE_INCLUDE
#define MKTAEEXTERN
bool _isInitialized{false};
int _frameNumber {0};

VkExtent2D _windowExtent {1000, 700};
int _selectedShader{0};

struct SDL_Window* _window{NULL};
#else
#define MKTAEEXTERN extern
MKTAEEXTERN bool _isInitialized;
MKTAEEXTERN int _frameNumber;

MKTAEEXTERN VkExtent2D _windowExtent;

MKTAEEXTERN int _selectedShader;

MKTAEEXTERN struct SDL_Window* _window;
#endif

MKTAEEXTERN MainDeletionQueue _mainDeletionQueue;

MKTAEEXTERN void _MKTGE_init();

MKTAEEXTERN void _MKTGE_cleanup();

MKTAEEXTERN void _MKTGE_draw();

MKTAEEXTERN void _MKTGE_run();

MKTAEEXTERN VkInstance _instance;
MKTAEEXTERN VkDebugUtilsMessengerEXT _debug_messenger;
MKTAEEXTERN VkPhysicalDevice _chosenGPU;
MKTAEEXTERN VkDevice _device;
MKTAEEXTERN VkSurfaceKHR _surface;
MKTAEEXTERN VkSwapchainKHR _swapchain;
MKTAEEXTERN VkFormat _swapchainImageFormat;
MKTAEEXTERN std::vector<VkImage> _swapchainImages;
MKTAEEXTERN std::vector<VkImageView> _swapchainImageViews;
MKTAEEXTERN VkQueue _graphicsQueue;
MKTAEEXTERN uint32_t _graphicsQueueFamily;
MKTAEEXTERN VkCommandPool _commandPool;
MKTAEEXTERN VkCommandBuffer _mainCommandBuffer;
MKTAEEXTERN VkRenderPass _renderPass;
MKTAEEXTERN std::vector<VkFramebuffer> _framebuffers;
MKTAEEXTERN VkSemaphore _presentSemaphore, _renderSemaphore;
MKTAEEXTERN VkFence _renderFence;
MKTAEEXTERN VkPipelineLayout _trianglePipelineLayout;
MKTAEEXTERN VkPipeline _trianglePipeline;
MKTAEEXTERN VkPipeline _CtrianglePipeline;
MKTAEEXTERN void _MKTGE_init_vulkan();
MKTAEEXTERN void _MKTGE_init_swapchain();
MKTAEEXTERN void _MKTGE_init_commands();
MKTAEEXTERN void _MKTGE_init_default_renderpass();
MKTAEEXTERN void _MKTGE_init_framebuffers();
MKTAEEXTERN void _MKTGE_init_sync_structures();
MKTAEEXTERN void _MKTGE_init_pipelines();
MKTAEEXTERN void _MKTGE_load_meshes();
MKTAEEXTERN void _MKTGE_load_AG();
MKTAEEXTERN void _MKTGE_upload_mesh(Mesh * mesh);
MKTAEEXTERN void _MKTGE_upload_AG(arrayGraphic * AG);
MKTAEEXTERN bool _MKTGE_load_shader_module(const char* filePath, VkShaderModule* outShaderModule);
MKTAEEXTERN std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
MKTAEEXTERN VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
MKTAEEXTERN VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
MKTAEEXTERN VkViewport _viewport;
MKTAEEXTERN VkRect2D _scissor;
MKTAEEXTERN VkPipelineRasterizationStateCreateInfo _rasterizer;
MKTAEEXTERN VkPipelineColorBlendAttachmentState _colorBlendAttachment;
MKTAEEXTERN VkPipelineMultisampleStateCreateInfo _multisampling;
MKTAEEXTERN VkPipelineLayout _pipelineLayout;
MKTAEEXTERN VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);
MKTAEEXTERN VkCommandPoolCreateInfo command_pool_create_info(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags);
MKTAEEXTERN VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool pool, uint32_t count, VkCommandBufferLevel level);
MKTAEEXTERN VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
MKTAEEXTERN VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info();
MKTAEEXTERN VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info(VkPrimitiveTopology topology);
MKTAEEXTERN VkPipelineRasterizationStateCreateInfo rasterization_state_create_info(VkPolygonMode polygonMode);
MKTAEEXTERN VkPipelineMultisampleStateCreateInfo multisampling_state_create_info();
MKTAEEXTERN VkPipelineColorBlendAttachmentState color_blend_attachment_state();
MKTAEEXTERN VkPipelineLayoutCreateInfo pipeline_layout_create_info();
MKTAEEXTERN VkFenceCreateInfo fence_create_info(VkFenceCreateFlags flags = 0);
MKTAEEXTERN VkSemaphoreCreateInfo semaphore_create_info(VkSemaphoreCreateFlags flags = 0);
MKTAEEXTERN VmaAllocator _allocator;
MKTAEEXTERN VkPipeline _meshPipeline;
MKTAEEXTERN VkPipeline _arrayGraphicsPipeline;
MKTAEEXTERN Mesh _triangleMesh;
MKTAEEXTERN arrayGraphic _CtriangleMesh;
MKTAEEXTERN VertexInputDescription get_vertex_description();
MKTAEEXTERN VertexInputDescription get_AGvertex_description();
MKTAEEXTERN void * __P;
MKTAEEXTERN void * __PP;