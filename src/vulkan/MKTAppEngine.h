#pragma once
#include "types.h"
#include <vector>
#include <cstdio>
#include <deque>
#include <functional>
#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_vulkan.h>

#include "init.h"
#include <fstream>

#include <vk-bootstrap/src/VkBootstrap.h>

#include "MKTMesh.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <unordered_map>

constexpr unsigned int FRAME_OVERLAP = 2;

struct MeshPushConstants {
    glm::vec4 data;
    glm::mat4 render_matrix;
};

struct AGPushConstants {
    glm::mat4 colourModification;
    glm::mat4 transformation;
};

typedef struct finalToRender{
    MKTAG AG;
    AGPushConstants AGPC;
} sub2MAKiT;

struct DeletionQueue
{
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

struct Material {
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
};

struct RenderObject {
	Mesh* mesh;

	Material* material;

	glm::mat4 transformMatrix;
};

struct GPUCameraData{
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 viewproj;
};

struct FrameData {
	VkSemaphore _presentSemaphore, _renderSemaphore;
	VkFence _renderFence;	

	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;

    AllocatedBuffer cameraBuffer;

    VkDescriptorSet globalDescriptor;
};

struct UploadContext {
	VkFence _uploadFence;
	VkCommandPool _commandPool;
	VkCommandBuffer _commandBuffer;
};

class VentumEngine {
public:
    
    bool _isInitialized{false};
    int _frameNumber {0};

    VkExtent2D _windowExtent {1738, 972};

    struct SDL_Window* _window{nullptr};

    void init();

    void cleanup();

    void draw();

    void run();

    VkInstance _instance;
    VkDebugUtilsMessengerEXT _debug_messenger;
    VkPhysicalDevice _chosenGPU;
    VkDevice _device;
    VkSurfaceKHR _surface;

    VkSwapchainKHR _swapchain;

    VkFormat _swapchainImageFormat;

    std::vector<VkImage> _swapchainImages;

    std::vector<VkImageView> _swapchainImageViews;

    VkQueue _graphicsQueue;
    uint32_t _graphicsQueueFamily;

    VkRenderPass _renderPass;

    std::vector<VkFramebuffer> _framebuffers;

    VkPipelineLayout _trianglePipelineLayout;

    VkPipelineLayout _AGPipelineLayout;

    VkPipeline _trianglePipeline;
    VkPipeline _pinkTrianglePipeline;
    VkPipeline _meshPipeline;
    VkPipeline _AGPipeline;
    Mesh _triangleMesh;
    MKTAG _HexAg;

    int _selectedShader{0};

    DeletionQueue _mainDeletionQueue;

    VmaAllocator _allocator;

    VkPipelineLayout _meshPipelineLayout;

    Mesh _DUCKMesh;

    VkImageView _depthImageView;
	AllocatedImage _depthImage;

    VkFormat _depthFormat;

    std::vector<RenderObject> _renderables;

    std::vector<sub2MAKiT> _AGA;
    std::vector<MKTAG> _TAGA;

    std::unordered_map<std::string,Material> _materials;
    std::unordered_map<std::string,Mesh> _meshes;

    Material* create_material(VkPipeline pipeline, VkPipelineLayout layout,const std::string& name);

    Material* get_material(const std::string& name);

    Mesh* get_mesh(const std::string& name);

    void draw_objects(VkCommandBuffer cmd,RenderObject* first, int count);

    void draw_AG(VkCommandBuffer cmd,sub2MAKiT* first, int count);
    FrameData _frames[FRAME_OVERLAP];

    FrameData& get_current_frame();

    AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

    VkDescriptorSetLayout _globalSetLayout;
    VkDescriptorPool _descriptorPool;

    UploadContext _uploadContext;

private:

    void init_vulkan();

    void init_swapchain();

    void init_commands();

    void init_default_renderpass();

    void init_framebuffers();

    void init_sync_structures();

    void init_pipelines();

    bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule);

    void load_meshes();

    void load_AG();

    void upload_mesh(Mesh& mesh);

    void upload_AG(MKTAG& AG);

    void init_scene();

	void init_descriptors();

    void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);
};

class PipelineBuilder {
public:
    
    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
    VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
    VkViewport _viewport;
    VkRect2D _scissor;
    VkPipelineRasterizationStateCreateInfo _rasterizer;
    VkPipelineColorBlendAttachmentState _colorBlendAttachment;
    VkPipelineMultisampleStateCreateInfo _multisampling;
    VkPipelineLayout _pipelineLayout;

    VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);

    VkPipelineDepthStencilStateCreateInfo _depthStencil;
};