#pragma once
#include <structs.h>
#include <../utils.h>
#include <../outerDefine.h>
#include <delQue/delQue.h>
#include <../errorHandling.h>
#include <types.h>
#include <utils/MKTbuffer.h>
#include <goodLuckMate/glm.h>

// MACROS

#define VK_CHECK(x)                                             \
do                                                              \
{                                                               \
    VkResult err = x;                                           \
    if (err)                                                    \
    {   \
        _sizeOfErrors++; _errors = realloc(_errors,_sizeOfErrors);_errors[_sizeOfErrors-1] = err;                                              \
        printf("Detected Vulkan error: %d\n",err);                                      \
        abort();                                                \
    }                                                           \
} while (0)

// STRUCTS

// GLM

typedef struct MKTDEVICERATING {
    IntDex rating;
    char isSuitable;
} MKTdeviceRating;

typedef struct MKTVOIDVECTOR {
    void * ptr;
    IntDex sizeOfPtr;
} MKTVvoid;

typedef const char * MKTString;

typedef struct MKTSTRINGVECTOR {
    MKTString * pString;
    IntDex sizeOfString;
} MKTVstring;

typedef struct MKTAGDESCRIPTOR {
    MKTmatf2 input;
} MKTAGdescriptor;

typedef struct MKTRUNTIMEKIT {

    VkCommandBuffer commandBuffer;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

} MKTVKruntime;

typedef struct MKTMATERIAL {
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkDescriptorSetLayout descriptorSetLayout;

    IntDex ID;
} MKTmaterial;

typedef struct MKTAG {
    MKTAGdata _dataAG;

    VkBuffer * uniformBuffers;
    VkDeviceMemory * uniformBuffersMemory;
    IntDex sizeOfUniformBuffers;

    VkDescriptorPool descriptorPool;
    VkDescriptorSet * descriptorSets;
    IntDex sizeOfDescriptorSets;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
} MKTag;

typedef struct MKTPIC {

    MKTPiCdata _dataPiC; // will contain a garbage value during runtime, only for init purposes

    VkImageView textureImageView;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkBuffer * uniformBuffers;
    VkDeviceMemory * uniformBuffersMemory;
    IntDex sizeOfUniformBuffers;

    VkDescriptorPool descriptorPool;
    VkDescriptorSet * descriptorSets;
    IntDex sizeOfDescriptorSets;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

} MKTPiC;

typedef struct MKTBM {

    MKTbmdata _dataPiC;

    VkImageView textureImageView;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkBuffer * uniformBuffers;
    VkDeviceMemory * uniformBuffersMemory;
    IntDex sizeOfUniformBuffers;

    VkDescriptorPool descriptorPool;
    VkDescriptorSet * descriptorSets;
    IntDex sizeOfDescriptorSets;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

} MKTbm;


typedef struct MKTTD {

    MKTTDdata _dataPiC;

    VkImageView textureImageView;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkBuffer * uniformBuffers;
    VkDeviceMemory * uniformBuffersMemory;
    IntDex sizeOfUniformBuffers;

    VkDescriptorPool descriptorPool;
    VkDescriptorSet * descriptorSets;
    IntDex sizeOfDescriptorSets;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

} MKTtd;

// FUNCTIONS

// MAIN FUNCTIONS
void init();
void run();
void cleanup();

// INIT FUNCTIONS
void _VE_INIT_window(); // An init function for GLFW window creation
void _VE_INIT_Instance(); // An init function for Instance creation
void _VE_INIT_PhysicalDevice(); // An init function for picking the physical device
void _VE_INIT_LogicalDevice(); // An init function for creating the logical device
void _VE_INIT_WindowSurface(); // An init function for creating the window surface
void _VE_INIT_Swapchain(); // An init function for creating the swapchain
void _VE_INIT_ImageViews(); // An init function for creating the image views
void _VE_INIT_RenderPass(); // An init function for creating the renderpass
void _VE_INIT_GraphicPipelines(); // An init function for creating the graphic pipelines
void _VE_INIT_Framebuffers(); // An init function for creating the framebuffers
void _VE_INIT_CommandPool(); // An init function for creating the command pool
void _VE_INIT_CommandBuffer(); // An init function for creating the command buffer
void _VE_INIT_SyncObjects(); // An init function for creating semaphores and fences
void _VE_INIT_Sampler(); // An init function for creating semaphores and fences
void _VE_INIT_VE(); // An init function for VentumEngine variables

void * _MKTGENERALFUNC(IntDex index, void * data);

void framebufferResizeCallback(GLFWwindow* window, int width, int height);

// RUNTIME FUNCTIONS
void _VE_RUN_recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
void _VE_RUN_drawFrame();
void _VE_RUN_updateDescriptors(void * data, IntDex dataSize, IntDex type, IntDex index);

// UTILS FUNCTIONS
void _VE_UTILS_RecreateSwapChain(); // An Utils function for reacreating the swapchain (in case the window size changes etc.)
void _VE_UTILS_CleanupSwapChain(); // An Utils function for cleaning up the swapchain (in case the swapchain is recreated)

// GLOBAL VARIABLES

#ifdef _MKT_APP_ENGINE_THIRD_
// Without the guard

// GLFW
GLFWwindow * _window;
uint32_t glfwExtensionCount = 0;
const char** glfwExtensions = NULL;

// VK_INIT
VkExtent2D _WindowS = {600,600};
int MAX_FRAMES_IN_FLIGHT = 2;
VkInstance _instance;
VkPhysicalDevice _chosenGPU = VK_NULL_HANDLE;
VkDevice _device;
VkDeviceQueueCreateInfo * queueCreateInfos = NULL;
IntDex sizeOfQueueCreateInfos;
VkSampler _textureSampler;

// VK_RUNTIME
VkSurfaceKHR _surface;
VkQueue _graphicsQueue;
VkQueue _presentQueue;
unsigned int _imageCount;
VkRenderPass _renderPass;
VkCommandPool _commandPool;
MKTVKruntime * _runtimeKit = NULL;
IntDex _sizeOfRuntimeKit;
VkCommandBuffer * _commandBuffers = NULL;
IntDex _sizeOfCommandBuffers;
IntDex _currentFrame = 0;
char _framebufferResized = 0;

// SWAPCHAIN
VkSwapchainKHR _swapChain;
VkImage * _swapChainImages = NULL;
IntDex _sizeOfSwapChainImages;
VkImageView * _swapChainImageViews = NULL;
IntDex _sizeOfSwapChainImageViews;
VkFormat _swapChainImageFormat;
VkExtent2D _swapChainExtent;
VkFramebuffer * _swapChainFramebuffers = NULL;
IntDex _sizeOfSwapChainFramebuffers;

// extensions
MKTVstring _requiredExtensions;
VkExtensionProperties * _extensions = NULL;
MKTVstring _deviceExtensions;

// RENDER
MKTmaterial * _ren_materials = NULL;
IntDex _ren_sizeOfMaterials;

MKTag * _ren_AG = NULL;
IntDex _ren_sizeOfAG;

MKTPiC * _ren_PiC = NULL;
IntDex _ren_sizeOfPiC;

MKTbm * _ren_BM = NULL;
IntDex _ren_sizeOfBM;


// validation layers
#ifdef NDEBUG
    const char enableValidationLayers = 0;
#else
    const char enableValidationLayers = 1;
#endif
MKTVstring validationLayers;

#else
// With the guard

// GLFW
extern GLFWwindow * _window;
extern uint32_t glfwExtensionCount;
extern const char** glfwExtensions;

// VK_INIT
extern VkExtent2D _WindowS;
extern int MAX_FRAMES_IN_FLIGHT;
extern VkInstance _instance;
extern VkPhysicalDevice _chosenGPU;
extern VkDevice _device;
extern VkDeviceQueueCreateInfo * queueCreateInfos;
extern IntDex sizeOfQueueCreateInfos;
extern VkSampler _textureSampler;

// VK_RUNTIME
extern VkSurfaceKHR _surface;
extern VkQueue _graphicsQueue;
extern VkQueue _presentQueue;
extern unsigned int _imageCount;
extern VkRenderPass _renderPass;
extern VkCommandPool _commandPool;
extern MKTVKruntime * _runtimeKit;
extern IntDex _sizeOfRuntimeKit;
extern VkCommandBuffer * _commandBuffers;
extern IntDex _sizeOfCommandBuffers;
extern IntDex _currentFrame;
extern char _framebufferResized;

// SWAPCHAIN
extern VkSwapchainKHR _swapChain;
extern VkImage * _swapChainImages;
extern IntDex _sizeOfSwapChainImages;
extern VkImageView * _swapChainImageViews;
extern IntDex _sizeOfSwapChainImageViews;
extern VkFormat _swapChainImageFormat;
extern VkExtent2D _swapChainExtent;
extern VkFramebuffer * _swapChainFramebuffers;
extern IntDex _sizeOfSwapChainFramebuffers;

// extensions
extern MKTVstring _requiredExtensions;
extern VkExtensionProperties * _extensions;
extern MKTVstring _deviceExtensions;

// RENDER
extern MKTmaterial * _ren_materials;
extern IntDex _ren_sizeOfMaterials;

// validation layers
extern const char enableValidationLayers;
extern MKTVstring validationLayers;

extern MKTag * _ren_AG;
extern IntDex _ren_sizeOfAG;

extern MKTPiC * _ren_PiC;
extern IntDex _ren_sizeOfPiC;

extern MKTbm * _ren_BM;
extern IntDex _ren_sizeOfBM;

#endif

extern unsigned long long _MKTAS_STAT(unsigned long long int * rsi);
extern unsigned long long _MKTAS_UIFAC(unsigned long long int rsi);
extern double _MKTAS_CALC_E(unsigned long long int rsi);

/* some helpfull errors to know
* 1. -1073741819 - The "Dev can't use malloc correctly" error
* 2. -1073740940 - Heap corruption error (fun fact, is almost impossible to debug with gdb)
* 3. -1073741510 - The physical resources of this disk have been exhausted.
*/