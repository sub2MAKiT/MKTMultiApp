#include <../utils.h>
#include <../outerDefine.h>
#include <delQue/delQue.h>
#include <../errorHandling.h>
#include <types.h>

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

// GLOBAL VARIABLES

#ifdef _MKT_APP_ENGINE_THIRD_
// Without the guard

// GLFW
GLFWwindow * _window;
uint32_t glfwExtensionCount = 0;
const char** glfwExtensions;


// VK_INIT
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

// SWAPCHAIN
VkSwapchainKHR _swapChain;
VkImage * _swapChainImages;
IntDex _sizeOfSwapChainImages;
VkImageView * _swapChainImageViews;
IntDex _sizeOfSwapChainImageViews;
VkFormat _swapChainImageFormat;
VkExtent2D _swapChainExtent;

// extensions
MKTVstring _requiredExtensions;
VkExtensionProperties * _extensions;
MKTVstring _deviceExtensions;


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
// SWAPCHAIN
extern VkSwapchainKHR _swapChain;
extern VkImage * _swapChainImages;
extern IntDex _sizeOfSwapChainImages;
extern VkImageView * _swapChainImageViews;
extern IntDex _sizeOfSwapChainImageViews;
extern VkFormat _swapChainImageFormat;
extern VkExtent2D _swapChainExtent;

// extensions
extern MKTVstring _requiredExtensions;
extern VkExtensionProperties * _extensions;
extern MKTVstring _deviceExtensions;



// validation layers
extern const char enableValidationLayers;
extern MKTVstring validationLayers;

#endif

/* some helpfull errors to know
* 1.
* 2.
* 3. -1073741510 - The physical resources of this disk have been exhausted.
*/