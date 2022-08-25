#pragma once
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
#define MKTGLMDOUBLE float

typedef struct MKTRGBA {
    MKTGLMDOUBLE r;
    MKTGLMDOUBLE g;
    MKTGLMDOUBLE b;
    MKTGLMDOUBLE a;
} MKTrgba;

typedef struct MKTXY {
    MKTGLMDOUBLE x;
    MKTGLMDOUBLE y;
} MKTxy;



typedef struct MKTAGVertex { // i = 0
    MKTxy pos;
    MKTrgba colour;
} AGVertex;

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

typedef struct MKTMATERIAL {
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    IntDex ID;
} MKTmaterial;

typedef struct MKTRUNTIMEKIT {

    VkCommandBuffer commandBuffer;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

} MKTVKruntime;

typedef struct MKTAG {
    AGVertex * vertices;
    IntDex sizeOfVertices;
    
    unsigned int * indices;
    IntDex sizeOfIndices;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
} MKTag; 

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
void _VE_INIT_VE(); // An init function for VentumEngine variables

// RUNTIME FUNCTIONS
void _VE_RUN_recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
void _VE_RUN_drawFrame();

// UTILS FUNCTIONS
void _VE_UTILS_RecreateSwapChain(); // An Utils function for reacreating the swapchain (in case the window size changes etc.)
void _VE_UTILS_CleanupSwapChain(); // An Utils function for cleaning up the swapchain (in case the swapchain is recreated)


// GLOBAL VARIABLES

#ifdef _MKT_APP_ENGINE_THIRD_
// Without the guard

// TEMP
MKTag triangle;

// GLFW
GLFWwindow * _window;
uint32_t glfwExtensionCount = 0;
const char** glfwExtensions;

// VK_INIT
int MAX_FRAMES_IN_FLIGHT = 2;
VkInstance _instance;
VkPhysicalDevice _chosenGPU = VK_NULL_HANDLE;
VkDevice _device;
VkDeviceQueueCreateInfo * queueCreateInfos;
IntDex sizeOfQueueCreateInfos;

// VK_RUNTIME
VkSurfaceKHR _surface;
VkQueue _graphicsQueue;
VkQueue _presentQueue;
unsigned int _imageCount;
VkRenderPass _renderPass;
VkCommandPool _commandPool;
MKTVKruntime * _runtimeKit;
IntDex _sizeOfRuntimeKit;
VkCommandBuffer * _commandBuffers;
IntDex _sizeOfCommandBuffers;
IntDex _currentFrame = 0;
char _framebufferResized = 0;

// SWAPCHAIN
VkSwapchainKHR _swapChain;
VkImage * _swapChainImages;
IntDex _sizeOfSwapChainImages;
VkImageView * _swapChainImageViews;
IntDex _sizeOfSwapChainImageViews;
VkFormat _swapChainImageFormat;
VkExtent2D _swapChainExtent;
VkFramebuffer * _swapChainFramebuffers;
IntDex _sizeOfSwapChainFramebuffers;

// extensions
MKTVstring _requiredExtensions;
VkExtensionProperties * _extensions;
MKTVstring _deviceExtensions;

// RENDER
MKTmaterial * _REN_materials;
IntDex _REN_sizeOfMaterials;


// validation layers
#ifdef NDEBUG
    const char enableValidationLayers = 0;
#else
    const char enableValidationLayers = 1;
#endif
MKTVstring validationLayers;

#else
// With the guard

// TEMP
extern MKTag triangle;

// GLFW
extern GLFWwindow * _window;
extern uint32_t glfwExtensionCount;
extern const char** glfwExtensions;

// VK_INIT
extern int MAX_FRAMES_IN_FLIGHT;
extern VkInstance _instance;
extern VkPhysicalDevice _chosenGPU;
extern VkDevice _device;
extern VkDeviceQueueCreateInfo * queueCreateInfos;
extern IntDex sizeOfQueueCreateInfos;

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
extern MKTmaterial * _REN_materials;
extern IntDex _REN_sizeOfMaterials;

// validation layers
extern const char enableValidationLayers;
extern MKTVstring validationLayers;

#endif

/* some helpfull errors to know
* 1.
* 2.
* 3. -1073741510 - The physical resources of this disk have been exhausted.
*/