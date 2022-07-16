#pragma once
#include "types.h"
#include <vector>
#include <cstdio>
#include <deque>
#include <functional>
#include <iostream>
#include "../libraryHeader.h"
#include "./_render.h"

#ifdef _WIN32
#include <windows.h>
#elif __gnu_linux__
#include <dlfcn.h>
#endif

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
} vPic;

typedef struct MKTRGBAPixel {
unsigned char R;
unsigned char G;
unsigned char B;
unsigned char A;   
} MKTPix;

typedef struct MKTRGBApicture {
MKTPix * pixels;
size_t width;
size_t height;
} MKTPic;

typedef struct MKTFILECONVERTERSTRUCT {
size_t sizeOfString;
char * string;
char type;
void*(*loadFile)(char * FP);
} MKTfileImporter;

extern GL * Modules;
extern size_t sizeOfModules;

int loadFile(char * filePath, char mode);
MKTPic _MKT_loadImage(char * filePath,char * error);
void addToRenderQueue(int ID);
void removeFromRenderQueue(int ID);
void init_fileConverter();
char * charStringUnconstant(const char * whyNot);


#ifdef _MKTENGINEINCLUDEGUARD_
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
int loading_MKTP_image(MKTPic data);
VkDevice _device;
VmaAllocator _allocator;
MKTfileImporter * _MKT_fileTypes;
size_t _MKT_fileTypesSupported;
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
VkPipelineLayout _meshPipelineLayout;
Mesh _DUCKMesh;
VkImageView _depthImageView;
AllocatedImage _depthImage;
VkFormat _depthFormat;
AGPushConstants _defaultPushConstants;
std::vector<MKTAG> _TAGA;
MKTPiC _defaultPictureRectangle;
RenderArray _render;
std::vector<sub2MAKiT> _AGA;
std::vector<vPic> _pictures;
std::vector<RenderObject> _renderables;
std::unordered_map<std::string,Material> _materials;
std::unordered_map<std::string,Mesh> _meshes;
Material* create_material(VkPipeline pipeline, VkPipelineLayout layout,const std::string& name);
Material* get_material(const std::string& name);
Mesh* get_mesh(const std::string& name);
void drawMenu(VkCommandBuffer cmd,GL * menuStuff, size_t sizeOfMenuStuff);
FrameData _frames[FRAME_OVERLAP];
FrameData& get_current_frame();
AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
VkDescriptorSetLayout _globalSetLayout;
VkDescriptorPool _descriptorPool;
size_t _currentPicturesDescriptorSize;
VkDescriptorSet * _currentPicturesDescriptor; // bad idea i know
VkDescriptorSetLayout _picturesSetLayout;
VkDescriptorPool _picturesDescriptorPool;
UploadContext _uploadContext;
Material AGMaterial;
void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);
float CBT; // current bar transformation
MKTPic testLogo;
void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
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
VkCommandBuffer beginSingleTimeCommands();
void endSingleTimeCommands(VkCommandBuffer commandBuffer);
void init_VentumEngineVariables();
void updateSpecificPictureDescriptor(vPic * Picture, size_t picureNumber);
#else
extern void CreatePictureDescriptors(vPic * pPic,size_t index);
extern void logoLoadingFunctionThatIsVeryImportant();
extern void MKTDRAW(VkCommandBuffer cmd);
extern void init_vulkan();
extern void init_swapchain();
extern void init_commands();
extern void init_default_renderpass();
extern void init_framebuffers();
extern void init_sync_structures();
extern void init_pipelines();
extern bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule);
extern void load_meshes();
extern void load_AG();
extern void loadMenuAG();
extern void upload_mesh(Mesh& mesh);
extern void init_scene();
extern void init_descriptors();
extern void init_createTextureSampler();
extern VkCommandBuffer beginSingleTimeCommands();
extern void endSingleTimeCommands(VkCommandBuffer commandBuffer);
extern void init_VentumEngineVariables();
extern void updateSpecificPictureDescriptor(vPic * Picture, size_t picureNumber);
extern uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
extern int loading_MKTP_image(MKTPic data);
extern VkDevice _device;
extern VmaAllocator _allocator;
extern MKTfileImporter * _MKT_fileTypes;
extern size_t _MKT_fileTypesSupported;
extern int imagesLoaded;
extern void upload_AG(MKTAG& AG);
extern bool _isInitialized;
extern int _frameNumber;
extern VkExtent2D _windowExtent;
extern struct SDL_Window* _window;
extern void init();
extern void cleanup();
extern void draw();
extern void run();
extern char modesEnabled;
extern VkInstance _instance;
extern VkDebugUtilsMessengerEXT _debug_messenger;
extern VkPhysicalDevice _chosenGPU;
extern VkSurfaceKHR _surface;
extern VkSwapchainKHR _swapchain;
extern VkFormat _swapchainImageFormat;
extern std::vector<VkImage> _swapchainImages;
extern std::vector<VkImageView> _swapchainImageViews;
extern VkSampler _textureSampler;
extern VkQueue _graphicsQueue;
extern uint32_t _graphicsQueueFamily;
extern VkRenderPass _renderPass;
extern std::vector<VkFramebuffer> _framebuffers;
extern VkPipelineLayout _trianglePipelineLayout;
extern VkPipelineLayout _AGPipelineLayout;
extern VkPipeline _trianglePipeline;
extern VkPipeline _pinkTrianglePipeline;
extern VkPipeline _meshPipeline;
extern VkPipeline _AGPipeline;
extern Mesh _triangleMesh;
extern MKTAG _HexAg;
extern VkPipelineLayout _DiSPipelineLayout;
extern VkPipeline _DiSPipeline;
extern int _selectedShader;
extern DeletionQueue _mainDeletionQueue;
extern VkPipelineLayout _meshPipelineLayout;
extern Mesh _DUCKMesh;
extern VkImageView _depthImageView;
extern AllocatedImage _depthImage;
extern VkFormat _depthFormat;
extern AGPushConstants _defaultPushConstants;
extern std::vector<MKTAG> _TAGA;
extern MKTPiC _defaultPictureRectangle;
extern RenderArray _render;
extern std::vector<sub2MAKiT> _AGA;
extern std::vector<vPic> _pictures;
extern std::vector<RenderObject> _renderables;
extern std::unordered_map<std::string,Material> _materials;
extern std::unordered_map<std::string,Mesh> _meshes;
extern Material* create_material(VkPipeline pipeline, VkPipelineLayout layout,const std::string& name);
extern Material* get_material(const std::string& name);
extern Mesh* get_mesh(const std::string& name);
extern void drawMenu(VkCommandBuffer cmd,GL * menuStuff, size_t sizeOfMenuStuff);
extern FrameData _frames[FRAME_OVERLAP];
extern FrameData& get_current_frame();
extern AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
extern VkDescriptorSetLayout _globalSetLayout;
extern VkDescriptorPool _descriptorPool;
extern size_t _currentPicturesDescriptorSize;
extern VkDescriptorSet * _currentPicturesDescriptor; // bad idea i know
extern VkDescriptorSetLayout _picturesSetLayout;
extern VkDescriptorPool _picturesDescriptorPool;
extern UploadContext _uploadContext;
extern Material AGMaterial;
extern void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);
extern float CBT; // current bar transformation
extern MKTPic testLogo;
extern void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
extern void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
extern void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
extern void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
extern void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
#endif

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