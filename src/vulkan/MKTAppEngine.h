#pragma once
#include "types.h"
#include <vector>
#include <cstdio>
#include <deque>
#include <functional>
#include <iostream>
#include "../libraryHeader.h"
#include "../fileManagment/MKTPicture.h"

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
    glm::vec3 movement;
    // glm::vec2 pivitPoint;
};

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

typedef struct defaultPicStruct {
    MKTPiC Pic;
    AGPushConstants AGPC;
}DefaultPic;

typedef struct finalToRender{
    MKTAG AG;
    AGPushConstants AGPC;
    char isVisible;
    // char * name;
    // size_t sizeOfName;
    // Material* material;
} sub2MAKiT;

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

typedef struct LoadedModule {
    sub2MAKiT icon;
    void (*entry)(functionInput);
} GL; // goodLuck

typedef struct vulPic {
VkImage textureImage;
VkDeviceMemory textureImageMemory;
VkImageView textureImageView;
AGPushConstants PC;
char isVisible;
VkDescriptorSet * _currentDescriptor; // bad idea i know
} vPic;

typedef struct MainRenderStruct {
sub2MAKiT* ArrayGraphics;
vPic* pictureGraphics;
RenderObject* Objects;

size_t sizeOfArrayGraphics;
size_t sizeOfPictureGraphics;
size_t sizeOfObjects;
} DrawStruct;

extern GL * Modules;
extern size_t sizeOfModules;

void addToRenderQueue(int ID);
void removeFromRenderQueue(int ID);


class VentumEngine {
public:
    
    int imagesLoaded;

    void upload_AG(MKTAG& AG);
    bool _isInitialized{false};
    int _frameNumber {0};

    VkExtent2D _windowExtent {1700, 800};

    struct SDL_Window* _window{nullptr};

    void init();

    void cleanup();

    void draw();

    void run();

    char modesEnabled;

    VkInstance _instance;
    VkDebugUtilsMessengerEXT _debug_messenger;
    VkPhysicalDevice _chosenGPU;
    VkDevice _device;
    VkSurfaceKHR _surface;

    VkSwapchainKHR _swapchain;

    VkFormat _swapchainImageFormat;

    std::vector<VkImage> _swapchainImages;

    std::vector<VkImageView> _swapchainImageViews;

    VkSampler _textureSampler;

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

    VkPipelineLayout _DiSPipelineLayout;
    VkPipeline _DiSPipeline;

    int _selectedShader{0};

    DeletionQueue _mainDeletionQueue;

    VmaAllocator _allocator;

    VkPipelineLayout _meshPipelineLayout;

    Mesh _DUCKMesh;

    VkImageView _depthImageView;
	AllocatedImage _depthImage;

    VkFormat _depthFormat;

    AGPushConstants _defaultPushConstants;

    std::vector<MKTAG> _TAGA;

    MKTPiC _defaultPictureRectangle;
    
    // IMPORTANT !!!
    std::vector<sub2MAKiT> _AGA;
    std::vector<vPic> _pictures;
    std::vector<RenderObject> _renderables;

    DrawStruct _drawQueue;

    std::unordered_map<std::string,Material> _materials;
    std::unordered_map<std::string,Mesh> _meshes;

    Material* create_material(VkPipeline pipeline, VkPipelineLayout layout,const std::string& name);

    Material* get_material(const std::string& name);

    Mesh* get_mesh(const std::string& name);

    void draw_objects(VkCommandBuffer cmd,RenderObject* first, int count);

    void draw_AG(VkCommandBuffer cmd,sub2MAKiT* first, int count);

    void drawMenu(VkCommandBuffer cmd,GL * menuStuff, size_t sizeOfMenuStuff);
    FrameData _frames[FRAME_OVERLAP];

    FrameData& get_current_frame();

    AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

    VkDescriptorSetLayout _globalSetLayout;
    VkDescriptorPool _descriptorPool;

    
    VkDescriptorSetLayout _picturesSetLayout;
    VkDescriptorPool _picturesDescriptorPool;

    UploadContext _uploadContext;

    Material AGMaterial;
    
    void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);

    float CBT; // current bar transformation

    MKTPic testLogo;

    int loading_MKTP_image(MKTPic data);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

private:

    void CreatePictureDescriptors(vPic * pPic,size_t index);

    void logoLoadingFunctionThatIsVeryImportant();

    void MKTDRAW(VkCommandBuffer cmd);

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

    void loadMenuAG();

    void upload_mesh(Mesh& mesh);

    void init_scene();

	void init_descriptors();

    void init_createTextureSampler();

    void draw_PiC(VkCommandBuffer cmd,vPic* first, int count);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void init_VentumEngineVariables();

    void updateSpecificPictureDescriptor(vPic * Picture, size_t picureNumber);
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