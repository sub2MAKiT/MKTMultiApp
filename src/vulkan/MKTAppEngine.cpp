#define _MKTENGINEINCLUDEGUARD_
#define MKTUTILSTHIRD
#include "MKTAppEngine.h"
#include "../fileManagment/MKTarrayGraphics.h"

#include "../DEBUG.h"
#ifdef _USE_GLFW
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32 //#ff0000
#endif

// Some basic graphics:

#define MKTDRAW3D 1
#define MKTDRAWAG 2
#define MKTDRAWPIC 4
#define MKTDRAWMENU 8
#define MKTDRAWTEXT 16

#define BLACKLINE_ID 0
#define MENUARRAYGRAPHIC 4
#define MENULINEHORIZONTAL 5

void init() {

    DEBUG("started init");

    #ifndef _USE_GLFW

    validationLayers = (const char**)malloc(sizeof(const char*));
    
    *validationLayers ={
    "VK_LAYER_KHRONOS_validation"
    };

    SDL_Init(SDL_INIT_VIDEO);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

    _window = SDL_CreateWindow(
        "MKTMultiApp", //#aaaa00
        SDL_WINDOWPOS_UNDEFINED, //#aaaa00
        SDL_WINDOWPOS_UNDEFINED, //#aaaa00
        _windowExtent.width, //#aaaa00
        _windowExtent.height, //#aaaa00
        window_flags //#aaaa00
    );
    #else
    glfwInit();\
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);\
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);\
    _window = glfwCreateWindow(_windowExtent.width, _windowExtent.height, "Vulkan", nullptr, nullptr);
    DEBUG("III init:GLFW III");
    #endif

    init_vulkan(); //#0000ff

    init_swapchain(); //#0000ff

    init_commands(); //#0000ff

    init_createTextureSampler(); // #0000ff

    init_default_renderpass(); //#0000ff

    init_framebuffers(); //#0000ff

    init_sync_structures(); //#0000ff
    
    init_fileConverter(); //#0000ff

    logoLoadingFunctionThatIsVeryImportant(); //#0000ff don't ask me why, but deleting this file will cause the whole engine to crash (I've made my own coconut.png)

    init_descriptors(); //#0000ff

    init_pipelines(); //#0000ff

    load_meshes(); //#0000ff

    load_AG(); //#0000ff

    loadMenuAG(); //#0000ff

    init_scene(); //#0000ff

    init_VentumEngineVariables(); //#0000ff

    /* 
    logoLoading
    load_AG
    loadMenuAG
    init_scene
    load_file
    */
    #ifdef _WIN32
    char _USEROS_ = 0;
    #elif __gnu_linux__
    char _USEROS_ = 1;
    #elif __APPLE__
    char _USEROS_ = 2;
    #endif

    initi initData = {};

    initData.userOS = _USEROS_;
    initData.windowX = _windowExtent.width;
    initData.windowY = _windowExtent.height;

    for(int i = 0; i < sizeOfModules;i++)
        Modules[i].init(initData);

    _isInitialized = true;
}

#ifndef NOVKB
void init_vulkan() //#0000ff
{
    vkb::InstanceBuilder builder;
    DEBUG("II started init:vulkan II");

    auto inst_ret = builder.set_app_name("MKTMultiApp")
        .request_validation_layers(true)//#aaaa00
        .require_api_version(1, 3, 216)//#aaaa00
        .use_default_debug_messenger()
		.build();
    DEBUG("created the builder");

    vkb::Instance vkb_inst = inst_ret.value();

    _instance = vkb_inst.instance;

    _debug_messenger = vkb_inst.debug_messenger;

    SDL_Vulkan_CreateSurface(_window, _instance, &_surface);

    vkb::PhysicalDeviceSelector selector{ vkb_inst };
	vkb::PhysicalDevice physicalDevice = selector
		.set_minimum_version(1, 3)
		.set_surface(_surface)
		.select()
		.value();

        vkb::DeviceBuilder deviceBuilder{ physicalDevice };

	    vkb::Device vkbDevice = deviceBuilder.build().value();

        _device = vkbDevice.device;
	    _chosenGPU = physicalDevice.physical_device;

        _graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
        _graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.physicalDevice = _chosenGPU;
        allocatorInfo.device = _device;
        allocatorInfo.instance = _instance;
        vmaCreateAllocator(&allocatorInfo, &_allocator);

    DEBUG("II init:vulkan II");
}

void init_swapchain() //#0000ff
{
    vkb::SwapchainBuilder swapchainBuilder{_chosenGPU,_device,_surface };

    DEBUG("started building a swapchain");
    vkb::Swapchain vkbSwapchain = swapchainBuilder
		.use_default_format_selection()
        .set_desired_present_mode(VK_PRESENT_MODE_MAILBOX_KHR)//VK_PRESENT_MODE_FIFO_KHR
        .set_desired_extent(_windowExtent.width, _windowExtent.height)
		.build()
		.value();

    DEBUG("swapchain built");
        _swapchain = vkbSwapchain.swapchain;
	    _swapchainImages = vkbSwapchain.get_images().value();
	    _swapchainImageViews = vkbSwapchain.get_image_views().value();

        _swapchainImageFormat = vkbSwapchain.image_format;

        _mainDeletionQueue.push_function([=]() {
            vkDestroySwapchainKHR(_device, _swapchain, nullptr);
        });

    DEBUG("swapchainImages sorted");
    VkExtent3D depthImageExtent = {
        _windowExtent.width,
        _windowExtent.height,
        1
    };

	_depthFormat = VK_FORMAT_D32_SFLOAT;

	VkImageCreateInfo dimg_info = vkinit::image_create_info(_depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImageExtent);

	// VmaAllocationCreateInfo dimg_allocinfo = {};
	// dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	// dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// vmaCreateImage(_allocator, &dimg_info, &dimg_allocinfo, &_depthImage._image, &_depthImage._allocation, nullptr);

	VkImageViewCreateInfo dview_info = vkinit::imageview_create_info(_depthFormat, _depthImage._image, VK_IMAGE_ASPECT_DEPTH_BIT);

	VK_CHECK(vkCreateImageView(_device, &dview_info, nullptr, &_depthImageView));

	_mainDeletionQueue.push_function([=]() {
		vkDestroyImageView(_device, _depthImageView, nullptr);
		vmaDestroyImage(_allocator, _depthImage._image, _depthImage._allocation);
	});

    DEBUG("II init:swapchain II");
}

#else

// char checkValidationLayerSupport() {
//     uint32_t layerCount;
//     vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

//     VkLayerProperties * availableLayers = (VkLayerProperties*)malloc(layerCount*sizeof(VkLayerProperties));
//     vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);
//     free(availableLayers);

//     for (const char* layerName : validationLayers) {
//     char layerFound = false;

//     for (const auto& layerProperties : availableLayers) {
//         if (MKTstrcmp(layerName, layerProperties.layerName) == 0) {
//             layerFound = true;
//             break;
//         }
//     }

//     if (!layerFound) {
//         return false;
//     }
// }

// return true;
// }

typedef struct QueueFI {
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    char isOptionalPresent;
    char isOptional;

    char isComplete() {
        return isOptional && isOptionalPresent;
    }
} QueueFamilyIndices;


QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;
    indices.isOptional = true;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    DEBUG("got the device Queue family properties");

    VkQueueFamilyProperties * queueFamilies = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties)*queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);
    DEBUG("got the device Queue family properties for the queue count");

    for (int i = 0; i < queueFamilyCount; i++)
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            DEBUG("queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT");
            indices.graphicsFamily = i;
            RDEBUG("1");
            indices.isOptional = false;
            RDEBUG("2");
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);
            RDEBUG("3");
            if (presentSupport) {
                indices.presentFamily = i;
            }
        }

    DEBUG("checked Queue family properties");

    return indices;
}


char checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);
    DEBUG("checking for the device extensions support");

    VkExtensionProperties * availableExtensions = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties)*extensionCount);
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, availableExtensions);

    // std::set<const char *> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    char shouldFail = 0;

    for(int i = 0; i < sizeOfDeviceExtensions&&shouldFail != 1;i++)
        for(int a = 0; a < extensionCount && shouldFail != 2; a++)
            if(!weakMKTstrcmp(deviceExtensions[i], availableExtensions[a].extensionName))//&&a==extensionCount-1)
            {
                // printf("%s %s %d %d\n",deviceExtensions[i],availableExtensions[a].extensionName,i,a);
                shouldFail = 1;
            } else {
                shouldFail = 2;
            }
    // availableExtensions[0].extensionName;

    // for (const auto& extension : availableExtensions) {
    //     requiredExtensions.erase(extension.extensionName);
    // }
    DEBUG("checked for the device extensions support");

    return shouldFail==0?1:(shouldFail==2?1:0);
}

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR * formats;
    size_t sizeOfFormats;
    VkPresentModeKHR * presentModes;
    size_t sizeOfPresentModes;
};

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const VkSurfaceFormatKHR * availableFormats, size_t sizeOfAvailableFormats) {
    for (int i = 0; i < sizeOfAvailableFormats;i++) {
        if (availableFormats[i].format == VK_FORMAT_R8G8B8A8_SRGB && availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormats[i];
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const VkPresentModeKHR * availablePresentModes, size_t sizeOfAvailablePresentModes) {
    for (int i = 0; i < sizeOfAvailablePresentModes;i++) {
        if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentModes[i];
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

long long MKTCLAMP(size_t a,size_t b,size_t c)
{
    return a<b?b:(a>c?c:a);
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != 4294967295) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<unsigned int>(width),
            static_cast<unsigned int>(height)
        };

        actualExtent.width = MKTCLAMP(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = MKTCLAMP(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;

        DEBUG(" SwapChainSupportDetails");
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);
        DEBUG("got physical device capabilities");

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.sizeOfFormats = formatCount;
        details.formats = (VkSurfaceFormatKHR*)malloc(details.sizeOfFormats*sizeof(VkSurfaceFormatKHR));
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats);
        DEBUG("got physical device surface formats");
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);
        DEBUG("got physical device present modes");

    if (presentModeCount != 0) {
        details.sizeOfPresentModes = presentModeCount;
        details.presentModes = (VkPresentModeKHR*)malloc(details.sizeOfPresentModes*sizeof(VkPresentModeKHR));
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes);
        DEBUG("got physical device surface present modes");
    }

    return details;
}

char isDeviceSuitable(VkPhysicalDevice device) {
    DEBUG("started checking for the device suitability");
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    DEBUG("got the device properties");
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    DEBUG("got the device features");

    char extensionsSupported = checkDeviceExtensionSupport(device);

    DEBUG("checking for swapchain adequaty");

    char swapChainAdequate = false;
    if (extensionsSupported) {
        DEBUG("extensions supported");
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = swapChainSupport.sizeOfPresentModes && swapChainSupport.sizeOfFormats;
    }

    DEBUG("adequatized the swapchain");

    printf("so: %d %d\n",extensionsSupported, swapChainAdequate);

    #ifdef BePicky
    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
           deviceFeatures.geometryShader && extensionsSupported && swapChainAdequate;
    #else
    return true && extensionsSupported  && swapChainAdequate;
    #endif
}

void init_vulkan() //#0000ff
{

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "MKTMultiApp";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "VentumEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1,8,1);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    DEBUG("created app info");

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    DEBUG("got glfw required extensions");

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;

    VK_CHECK(vkCreateInstance(&createInfo, NULL, &_instance));

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    VkExtensionProperties * extensions = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties)*extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions);

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

    DEBUG("enumerated physical devices for the size");

    if (deviceCount == 0) {
        DEBUG("no GPU with vulkan support"); //error
    }

    VkPhysicalDevice * devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice)*deviceCount);
    vkEnumeratePhysicalDevices(_instance, &deviceCount, devices);
    DEBUG("enumerated physical devices");

    deviceExtensions = (char **)malloc(sizeof(char *));
    deviceExtensions[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    sizeOfDeviceExtensions = 1;
    DEBUG("setup the device extensions");

    #ifdef _USE_GLFW
    glfwCreateWindowSurface(_instance, _window, nullptr, &_surface);
    #else
    SDL_Vulkan_CreateSurface(_window, _instance, &_surface);
    #endif

    DEBUG("created the surface");

    for (int i = 0; i < deviceCount; i++) {
        if (isDeviceSuitable(devices[i])) {
            DEBUG("Found the GPU");
            _chosenGPU = devices[i];
            break;
        }
    }

    DEBUG("picked the chosen GPU");

    if (_chosenGPU == VK_NULL_HANDLE) {
        DEBUG("device is nonexistant");//error
    }

    QueueFamilyIndices indices = findQueueFamilies(_chosenGPU);

    DEBUG("found the queue families");

    sizeOfUQF = 2;
    unsigned int uniqueQueueFamilies[sizeOfUQF] = {indices.graphicsFamily, indices.presentFamily};
    DEBUG("created the uniqueQueueFamilies");

    float queuePriority = 1.0f;
    for (int i = 0; i < sizeOfUQF; i++) {
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = uniqueQueueFamilies[i];
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos = (VkDeviceQueueCreateInfo*)realloc(queueCreateInfos,sizeof(VkDeviceQueueCreateInfo)*(i+1));
    queueCreateInfos[i] = queueCreateInfo;
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo DcreateInfo{};
    DcreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    DcreateInfo.queueCreateInfoCount = 1;
    DcreateInfo.pEnabledFeatures = &deviceFeatures;
    DcreateInfo.enabledExtensionCount = (uint32_t)sizeOfDeviceExtensions;
    DcreateInfo.ppEnabledExtensionNames = deviceExtensions;
    DcreateInfo.queueCreateInfoCount = sizeOfUQF;
    DcreateInfo.pQueueCreateInfos = queueCreateInfos;

    VK_CHECK(vkCreateDevice(_chosenGPU, &DcreateInfo, nullptr, &_device));

    DEBUG("created the device");

    vkGetDeviceQueue(_device, indices.graphicsFamily, 0, &_graphicsQueue);
    vkGetDeviceQueue(_device, indices.presentFamily, 0, &_presentQueue);


    // if (enableValidationLayers) {
    //     createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    //     createInfo.ppEnabledLayerNames = validationLayers.data();
    // } else {
    //     createInfo.enabledLayerCount = 0;
    // }
    // VmaAllocatorCreateInfo allocatorInfo = {};
    // allocatorInfo.physicalDevice = _chosenGPU;
    // allocatorInfo.device = _device;
    // allocatorInfo.instance = _instance;

    // vmaCreateAllocator(&allocatorInfo, &_allocator);

    // #ifdef _WIN32
    // VkWin32SurfaceCreateInfoKHR createInfo{};
    // createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    // createInfo.hwnd = glfwGetWin32Window(_window);
    // createInfo.hinstance = GetModuleHandle(nullptr);
    // VK_CHECK(vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface));

    // #elif __gnu_linux__

    // #endif

    DEBUG("created the surface");

    _depthFormat = VK_FORMAT_D32_SFLOAT;

    VkExtent3D depthImageExtent = {
        _windowExtent.width,
        _windowExtent.height,
        1
    };

    VkImageCreateInfo dimg_info = vkinit::image_create_info(_depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImageExtent);

    DEBUG("created the depth image info");

	vkCreateImage(_device, &dimg_info,NULL,&_depthImage);

	// VkImageViewCreateInfo dview_info = vkinit::imageview_create_info(_depthFormat, _depthImage._image, VK_IMAGE_ASPECT_DEPTH_BIT);

    VkImageViewCreateInfo dview_info = vkinit::imageview_create_info(_depthFormat, _depthImage, VK_IMAGE_ASPECT_DEPTH_BIT);

    DEBUG("created the depth image view info");

    VK_CHECK(vkCreateImageView(_device, &dview_info, nullptr, &_depthImageView));

    DEBUG("created the depth image view");

    _mainDeletionQueue.push_function([=]() {
        vkDestroyImageView(_device, _depthImageView, nullptr);
    });
    
    DEBUG("finished the vulkan init");
    free(devices);
    free(extensions);
}

void init_swapchain() //#0000ff
{
    DEBUG("III started init swapchain III");

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(_chosenGPU);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats, swapChainSupport.sizeOfFormats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes,swapChainSupport.sizeOfPresentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    unsigned int imageCount = swapChainSupport.capabilities.minImageCount;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    printf("image: %d\n",imageCount);

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(_chosenGPU);
    uint32_t queueFamilyIndices[2] = {indices.graphicsFamily, indices.presentFamily};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VK_CHECK(vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapchain));

    vkGetSwapchainImagesKHR(_device, _swapchain, &imageCount, nullptr);
    _swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_device, _swapchain, &imageCount, _swapchainImages.data());

    _swapchainImageFormat = surfaceFormat.format;
    _swapChainExtent = extent;

    _mainDeletionQueue.push_function([=]() {
    vkDestroySwapchainKHR(_device, _swapchain, nullptr);
    });

    DEBUG("III init:swapchain III");
}
/*
things that we will need:

_instance
_debug_messenger
_surface
_device
_chosenGPU
_graphicsQueue
_graphicsQueueFamily

_swapchain
_swapchainImages
_swapchainImageViews 
_swapchainImageFormat



_mainDeletionQueue.push_function([=]() {
    vkDestroySwapchainKHR(_device, _swapchain, nullptr);
});

_depthFormat = VK_FORMAT_D32_SFLOAT;

VkImageCreateInfo dimg_info = vkinit::image_create_info(_depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImageExtent);

VmaAllocationCreateInfo dimg_allocinfo = {};
dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

vmaCreateImage(_allocator, &dimg_info, &dimg_allocinfo, &_depthImage._image, &_depthImage._allocation, nullptr);

VkImageViewCreateInfo dview_info = vkinit::imageview_create_info(_depthFormat, _depthImage._image, VK_IMAGE_ASPECT_DEPTH_BIT);

VK_CHECK(vkCreateImageView(_device, &dview_info, nullptr, &_depthImageView));

_mainDeletionQueue.push_function([=]() {
    vkDestroyImageView(_device, _depthImageView, nullptr);
    vmaDestroyImage(_allocator, _depthImage._image, _depthImage._allocation);
});



VmaAllocatorCreateInfo allocatorInfo = {};
allocatorInfo.physicalDevice = _chosenGPU;
allocatorInfo.device = _device;
allocatorInfo.instance = _instance;
vmaCreateAllocator(&allocatorInfo, &_allocator);

*/

#endif

void cleanup() {
    if (_isInitialized) {
        freeRenderArray(&_render);

        vkWaitForFences(_device, 1, &get_current_frame()._renderFence, true, 1000000000);

        _mainDeletionQueue.flush();

        vkDestroySurfaceKHR(_instance, _surface, nullptr);

        vkDestroyDevice(_device, nullptr);
        vkDestroyInstance(_instance, nullptr);
        #ifdef _USE_GLFW
        glfwDestroyWindow(_window);
        glfwTerminate();
        #else
        SDL_DestroyWindow(_window);
        #endif
        DEBUG("III cleaned up III");
    }
}

void draw() {
    //it don't be drawing
    RDEBUG("started drawing");
    VK_CHECK(vkWaitForFences(_device, 1, &get_current_frame()._renderFence, true, 1000000000));
    VK_CHECK(vkResetFences(_device, 1, &get_current_frame()._renderFence));
    RDEBUG("reseted the fences");

    uint32_t swapchainImageIndex;
    VK_CHECK(vkAcquireNextImageKHR(_device, _swapchain, 1000000000, get_current_frame()._presentSemaphore, nullptr, &swapchainImageIndex));
    printf("value: %d\n",swapchainImageIndex);
    RDEBUG("acquired next KHR image");

    VK_CHECK(vkResetCommandBuffer(get_current_frame()._mainCommandBuffer, 0));

    VkCommandBuffer cmd = get_current_frame()._mainCommandBuffer;

    VkCommandBufferBeginInfo cmdBeginInfo = {};
    cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBeginInfo.pNext = nullptr;

    cmdBeginInfo.pInheritanceInfo = nullptr;
    cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));
    RDEBUG("begun the command buffer drawing");

    VkClearValue clearValue;
    clearValue.color = {{0.0f, 0.0f, 1.0f, 1.0f}};
    VkClearValue depthClear;
    depthClear.depthStencil.depth = 1.f;

    VkRenderPassBeginInfo rpInfo = {};
    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpInfo.pNext = nullptr;

    rpInfo.renderPass = _renderPass;
    rpInfo.renderArea.offset.x = 0;
    rpInfo.renderArea.offset.y = 0;
    RDEBUG("5");
    rpInfo.renderArea.extent = _windowExtent;
    rpInfo.framebuffer = _framebuffers[swapchainImageIndex];

    rpInfo.clearValueCount = 2;
    RDEBUG("6");

    VkClearValue clearValues[] = {clearValue, depthClear};

    rpInfo.pClearValues = &clearValues[0];

    RDEBUG("before starting the render pass");
    vkCmdBeginRenderPass(cmd, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);
    RDEBUG("III before MKTDRAW III");

//#00ff00
//#00ff00
        MKTDRAW(cmd);
//#00ff00
//#00ff00
    RDEBUG("III after MKTDRAW III");

    vkCmdEndRenderPass(cmd);
    RDEBUG("ended the render pass");

    VK_CHECK(vkEndCommandBuffer(cmd));
    RDEBUG("ended the command buffer");

    VkSubmitInfo submit = {};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.pNext = nullptr;

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    submit.pWaitDstStageMask = &waitStage;

    submit.waitSemaphoreCount = 1;
    submit.pWaitSemaphores = &get_current_frame()._presentSemaphore;

    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = &get_current_frame()._renderSemaphore;

    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &cmd;

    RDEBUG("before submitting the queue");//note, the device might be lost before that function
    VK_CHECK(vkQueueSubmit(_graphicsQueue, 1, &submit, get_current_frame()._renderFence));
    RDEBUG("submitted the queue");// second note, VK_ERROR_DEVICE_LOST may be caused by missynchronisation
    // third note, dies on second swapchainImageIndex 1

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.swapchainCount = 1;
    presentInfo.pWaitSemaphores = &get_current_frame()._renderSemaphore;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pSwapchains = &_swapchain;

    presentInfo.pImageIndices = &swapchainImageIndex;

    VK_CHECK(vkQueuePresentKHR(_graphicsQueue, &presentInfo));

    RDEBUG("finished drawing");
    _frameNumber++;
}

void run() {
    #ifndef _USE_GLFW
    SDL_Event e;
    bool bQuit = false;

    DEBUG("III started running III");

    float xSize = 0;
    float ySize = 0;
    float xMove = 0;
    float yMove = 0;
    int currentObjectToDraw = 2;
    char isVisible;

    while (!bQuit)
    {
        draw();
    DEBUG("III is running III");
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) bQuit = true;
            else if (e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_SPACE) // temp
                    if(_render.sizePerArray[0] == 1)
                        addToRenderArray(&_render,1, _pictures.size()-1, 0);
                    else
                        removeRenderObject(&_render,0,1,1);
                else if(e.key.keysym.sym == SDLK_w)
                    CBTStatus += 0.01;
                else if(e.key.keysym.sym == SDLK_s)
                    CBTStatus -= 0.01;

            }
            else if(e.type == SDL_MOUSEWHEEL)
                    CBT += e.wheel.y < 0?0.01:-0.01;
            else if(e.type == SDL_MOUSEBUTTONDOWN)
                if(e.button.button == SDL_BUTTON_LEFT)
                    printf("is it a float %f or an int %d",e.button.x,e.button.x);
                
            

            if(currentObjectToDraw > 2)
                currentObjectToDraw = 0;

        }
    }
    #else
    while (!glfwWindowShouldClose(_window)) {
        draw();
        glfwPollEvents();
    }
    #endif
    DEBUG("III ran III");
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

void init_commands()
{
	VkCommandPoolCreateInfo commandPoolInfo = vkinit::command_pool_create_info(_graphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	
	for (int i = 0; i < FRAME_OVERLAP; i++) {

	
		VK_CHECK(vkCreateCommandPool(_device, &commandPoolInfo, nullptr, &_frames[i]._commandPool));

		VkCommandBufferAllocateInfo cmdAllocInfo = vkinit::command_buffer_allocate_info(_frames[i]._commandPool, 1);

		VK_CHECK(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &_frames[i]._mainCommandBuffer));

		_mainDeletionQueue.push_function([=]() {
			vkDestroyCommandPool(_device, _frames[i]._commandPool, nullptr);
		});
	}


	VkCommandPoolCreateInfo uploadCommandPoolInfo = vkinit::command_pool_create_info(_graphicsQueueFamily);
	VK_CHECK(vkCreateCommandPool(_device, &uploadCommandPoolInfo, nullptr, &_uploadContext._commandPool));

	_mainDeletionQueue.push_function([=]() {
		vkDestroyCommandPool(_device, _uploadContext._commandPool, nullptr);
	});

	VkCommandBufferAllocateInfo cmdAllocInfo = vkinit::command_buffer_allocate_info(_uploadContext._commandPool, 1);

	VK_CHECK(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &_uploadContext._commandBuffer));

    DEBUG("II init:commands II");
}

void init_default_renderpass()
{
    VkAttachmentDescription color_attachment = {};
	color_attachment.format = _swapchainImageFormat;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference color_attachment_ref = {};
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depth_attachment = {};
	depth_attachment.flags = 0;
	depth_attachment.format = _depthFormat;
	depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depth_attachment_ref = {};
	depth_attachment_ref.attachment = 1;
	depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;
	subpass.pDepthStencilAttachment = &depth_attachment_ref;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency depth_dependency = {};
	depth_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	depth_dependency.dstSubpass = 0;
	depth_dependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	depth_dependency.srcAccessMask = 0;
	depth_dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	depth_dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency dependencies[2] = { dependency, depth_dependency };

	VkAttachmentDescription attachments[2] = { color_attachment,depth_attachment };

	VkRenderPassCreateInfo render_pass_info = {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.attachmentCount = 2;
	render_pass_info.pAttachments = attachments;
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;
	render_pass_info.dependencyCount = 2;
	render_pass_info.pDependencies = dependencies;

    VK_CHECK(vkCreateRenderPass(_device, &render_pass_info, nullptr, &_renderPass));

    _mainDeletionQueue.push_function([=]() {
        vkDestroyRenderPass(_device, _renderPass, nullptr);
    });

    DEBUG("II init:renderPass II");
}

void init_framebuffers()
{
    VkFramebufferCreateInfo fb_info = {};
	fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fb_info.pNext = nullptr;

	fb_info.renderPass = _renderPass; // this failes, time to fix it #ff0000
	fb_info.attachmentCount = 1;
	fb_info.width = _windowExtent.width;
	fb_info.height = _windowExtent.height;
	fb_info.layers = 1;

    _swapchainImageViews = (VkImageView*)malloc(_swapchainImages.size()*sizeof(VkImageView));

    // for (size_t i = 0; i < _swapchainImages.size(); i++) {

        // }


    const uint32_t swapchain_imagecount = _swapchainImages.size();
	_framebuffers = std::vector<VkFramebuffer>(swapchain_imagecount);
    printf("swapchain_imagecount: %d\n",swapchain_imagecount);

    for (int i = 0; i < swapchain_imagecount; i++)
    {
        VkImageViewCreateInfo createInfo = {}; // #0000ff
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = _swapchainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = _swapchainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VK_CHECK(vkCreateImageView(_device, &createInfo, NULL, &_swapchainImageViews[i]));
    // error cleanup vkDestroyImageView(device, imageView, nullptr);
    

        VkImageView * attachments = (VkImageView*)malloc(sizeof(VkImageView)*2);
		RDEBUG("test Attachments 1");
        attachments[0] = _swapchainImageViews[i];
		RDEBUG("test Attachments 2");
        attachments[1] = _depthImageView;
		RDEBUG("test Attachments 3");

        
        fb_info.pAttachments = attachments;
        fb_info.attachmentCount = 2;

        DEBUG("before creating a framebuffer");
        VK_CHECK(vkCreateFramebuffer(_device, &fb_info, nullptr, &_framebuffers[i]));
        DEBUG("after creating a framebuffer");
        _mainDeletionQueue.push_function([=]() {
            vkDestroyFramebuffer(_device, _framebuffers[i], nullptr);
            vkDestroyImageView(_device, _swapchainImageViews[i], nullptr);
        });
        free(attachments);
	}
}

void init_sync_structures()
{
    VkFenceCreateInfo fenceCreateInfo = vkinit::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);

	VkSemaphoreCreateInfo semaphoreCreateInfo = vkinit::semaphore_create_info();

	for (int i = 0; i < FRAME_OVERLAP; i++) {

		VK_CHECK(vkCreateFence(_device, &fenceCreateInfo, nullptr, &_frames[i]._renderFence));

	_mainDeletionQueue.push_function([=]() {
		vkDestroyFence(_device, _frames[i]._renderFence, nullptr);
		});


	VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_frames[i]._presentSemaphore));
	VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_frames[i]._renderSemaphore));

	_mainDeletionQueue.push_function([=]() {
		vkDestroySemaphore(_device, _frames[i]._presentSemaphore, nullptr);
		vkDestroySemaphore(_device, _frames[i]._renderSemaphore, nullptr);
		});
	}

	
	VkFenceCreateInfo uploadFenceCreateInfo = vkinit::fence_create_info();

	VK_CHECK(vkCreateFence(_device, &uploadFenceCreateInfo, nullptr, &_uploadContext._uploadFence));
	_mainDeletionQueue.push_function([=]() {
		vkDestroyFence(_device, _uploadContext._uploadFence, nullptr);
	});

    DEBUG("II init:sync structures II");
}

bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule)
{
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        DEBUG("XX load:shaders failed XX");
        return false;
    }

    size_t fileSize = (size_t)file.tellg();

    std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

    file.seekg(0);

    file.read((char*)buffer.data(), fileSize);

    file.close();

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;

    createInfo.codeSize = buffer.size() * sizeof(uint32_t);
    createInfo.pCode = buffer.data();

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        return false;
    }
    *outShaderModule = shaderModule;
    DEBUG("II load:shaders II");
    return true;
}

void init_pipelines() {
    VkShaderModule triangleFragShader;
    if (!load_shader_module("./shaders/triangle.frag.spv", &triangleFragShader))
    {
        DEBUG("triangle frag error");
    }
    else {
        DEBUG("triangle frag loaded");
    }

    VkShaderModule triangleVertexShader;
    if (!load_shader_module("./shaders/triangle.vert.spv", &triangleVertexShader))
    {
        DEBUG("triangle vert error");
    }
    else {
        DEBUG("triangle vert loaded");
    }

    VkShaderModule pinkTriangleFragShader;
    if (!load_shader_module("./shaders/Ctriangle.frag.spv", &pinkTriangleFragShader))
    {
        DEBUG("triangle frag error");
    }
    else {
        DEBUG("triangle frag loaded");
    }

    VkShaderModule pinkTriangleVertexShader;
    if (!load_shader_module("./shaders/Ctriangle.vert.spv", &pinkTriangleVertexShader))
    {
        DEBUG("triangle vert error");
    }
    else {
        DEBUG("triangle vert loaded");
    }


    VkPipelineLayoutCreateInfo pipeline_layout_info = vkinit::pipeline_layout_create_info();

    VK_CHECK(vkCreatePipelineLayout(_device, &pipeline_layout_info, nullptr, &_trianglePipelineLayout));
    
    PipelineBuilder pipelineBuilder;

    pipelineBuilder._shaderStages.push_back(
        vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, triangleVertexShader)
    );

    pipelineBuilder._shaderStages.push_back(
        vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, triangleFragShader)
    );

    pipelineBuilder._vertexInputInfo = vkinit::vertex_input_state_create_info();

    pipelineBuilder._inputAssembly = vkinit::input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    pipelineBuilder._viewport.x = 0.0f;
    pipelineBuilder._viewport.y = 0.0f;
    pipelineBuilder._viewport.width = (float)_windowExtent.width;
    pipelineBuilder._viewport.height = (float)_windowExtent.height;
    pipelineBuilder._viewport.minDepth = 0.0f;
    pipelineBuilder._viewport.maxDepth = 1.0f;

    pipelineBuilder._scissor.offset = {0, 0};
    pipelineBuilder._scissor.extent = _windowExtent;

    pipelineBuilder._rasterizer = vkinit::rasterization_state_create_info(VK_POLYGON_MODE_FILL);

    pipelineBuilder._multisampling = vkinit::multisampling_state_create_info();

    pipelineBuilder._colorBlendAttachment = vkinit::color_blend_attachment_state();

    pipelineBuilder._pipelineLayout = _trianglePipelineLayout;

    pipelineBuilder._depthStencil = vkinit::depth_stencil_create_info(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);

    _trianglePipeline = pipelineBuilder.build_pipeline(_device, _renderPass);

    //     //--------------------------------------
    // //--------------------------------------
    // //--------------------------------------
    // //--------------------------------------
    // //--------------------------------------

    VkPipelineLayoutCreateInfo DiS_pipeline_layout_info = {};
    DiS_pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    DiS_pipeline_layout_info.pNext = nullptr;

    DiS_pipeline_layout_info.flags = 0;
    DiS_pipeline_layout_info.setLayoutCount = 1;
    DiS_pipeline_layout_info.pSetLayouts = &_picturesSetLayout;

    VkPushConstantRange DiSpush_constant;
	DiSpush_constant.offset = 0;
	DiSpush_constant.size = sizeof(AGPushConstants);
	DiSpush_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	DiS_pipeline_layout_info.pPushConstantRanges = &DiSpush_constant;
	DiS_pipeline_layout_info.pushConstantRangeCount = 1;

    VK_CHECK(vkCreatePipelineLayout(_device, &DiS_pipeline_layout_info, nullptr, &_DiSPipelineLayout));

    VertexInputDescription DiSvertexDescription = Vertex::getPiC_vertex_description();

	pipelineBuilder._vertexInputInfo.pVertexAttributeDescriptions = DiSvertexDescription.attributes;
	pipelineBuilder._vertexInputInfo.vertexAttributeDescriptionCount = DiSvertexDescription.sizeOfAttributes;

	pipelineBuilder._vertexInputInfo.pVertexBindingDescriptions = DiSvertexDescription.bindings;
	pipelineBuilder._vertexInputInfo.vertexBindingDescriptionCount = DiSvertexDescription.sizeOfBindings;

	pipelineBuilder._shaderStages.clear();

    VkShaderModule DiSFragShader;
    if (!load_shader_module("./shaders/DiS.frag.spv", &DiSFragShader))
    {
        DEBUG("DiS frag error");
    }
    else {
        DEBUG("DiS frag loaded");
    }


    VkShaderModule DiSVertShader;
	if (!load_shader_module("./shaders/DiS.vert.spv", &DiSVertShader))
	{
		DEBUG("Error when building the DiS vertex shader module");
	}
	else {
		DEBUG("DiS vertex shader successfully loaded");
	}

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, DiSVertShader));

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, DiSFragShader));

    pipelineBuilder._pipelineLayout = _DiSPipelineLayout;

    PipelineBuilder PB; // jelly

    DEBUG("Peanut buffer jelly time");

    pipelineBuilder._vertexInputInfo = vkinit::vertex_input_state_create_info();

    pipelineBuilder._inputAssembly = vkinit::input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    pipelineBuilder._viewport.x = 0.0f;
    pipelineBuilder._viewport.y = 0.0f;
    pipelineBuilder._viewport.width = (float)_windowExtent.width;
    pipelineBuilder._viewport.height = (float)_windowExtent.height;
    pipelineBuilder._viewport.minDepth = 0.0f;
    pipelineBuilder._viewport.maxDepth = 1.0f;

    pipelineBuilder._scissor.offset = {0, 0};
    pipelineBuilder._scissor.extent = _windowExtent;

    pipelineBuilder._rasterizer = vkinit::rasterization_state_create_info(VK_POLYGON_MODE_FILL);

    pipelineBuilder._multisampling = vkinit::multisampling_state_create_info();

    pipelineBuilder._colorBlendAttachment = vkinit::color_blend_attachment_state();

    pipelineBuilder._depthStencil = vkinit::depth_stencil_create_info(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);

    DEBUG("before creating the _DiSPipeline");
    // #ff0000
    _DiSPipeline = pipelineBuilder.build_pipeline(_device, _renderPass);
    // #ff0000
    DEBUG("after creating the _DiSPipeline");


    VkPipelineLayoutCreateInfo fQS_pipeline_layout_info = {};
    fQS_pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    fQS_pipeline_layout_info.pNext = nullptr;

    fQS_pipeline_layout_info.flags = 0;
    fQS_pipeline_layout_info.setLayoutCount = 1;
    fQS_pipeline_layout_info.pSetLayouts = &_picturesSetLayout;

    VkPushConstantRange fQSpush_constant;
	fQSpush_constant.offset = 0;
	fQSpush_constant.size = sizeof(AGPushConstants);
	fQSpush_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	fQS_pipeline_layout_info.pPushConstantRanges = &fQSpush_constant;
	fQS_pipeline_layout_info.pushConstantRangeCount = 1;

    VK_CHECK(vkCreatePipelineLayout(_device, &fQS_pipeline_layout_info, nullptr, &_fQSPipelineLayout));

    VertexInputDescription fQSvertexDescription = Vertex::getfQS_vertex_description();

	pipelineBuilder._vertexInputInfo.pVertexAttributeDescriptions = fQSvertexDescription.attributes;
	pipelineBuilder._vertexInputInfo.vertexAttributeDescriptionCount = fQSvertexDescription.sizeOfAttributes;

	pipelineBuilder._vertexInputInfo.pVertexBindingDescriptions = fQSvertexDescription.bindings;
	pipelineBuilder._vertexInputInfo.vertexBindingDescriptionCount = fQSvertexDescription.sizeOfBindings;

	pipelineBuilder._shaderStages.clear();

    DEBUG("cleared the pipeline builder after the DiS Vert shader");

    VkShaderModule fQSFragShader;
    if (!load_shader_module("./shaders/DiS.frag.spv", &fQSFragShader))
    {
        DEBUG("fQS frag error");
    }
    else {
        DEBUG("fQS frag loaded");
    }


    VkShaderModule fQSVertShader;
	if (!load_shader_module("./shaders/DiS.vert.spv", &fQSVertShader))
	{
		DEBUG("Error when building the fQS vertex shader module");
	}
	else {
		DEBUG("fQS vertex shader successfully loaded");
	}

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, fQSVertShader));

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, fQSFragShader));

    pipelineBuilder._pipelineLayout = _fQSPipelineLayout;

    DEBUG("PipelineBuilder FQs");

    pipelineBuilder._vertexInputInfo = vkinit::vertex_input_state_create_info();

    pipelineBuilder._inputAssembly = vkinit::input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    pipelineBuilder._viewport.x = 0.0f;
    pipelineBuilder._viewport.y = 0.0f;
    pipelineBuilder._viewport.width = (float)_windowExtent.width;
    pipelineBuilder._viewport.height = (float)_windowExtent.height;
    pipelineBuilder._viewport.minDepth = 0.0f;
    pipelineBuilder._viewport.maxDepth = 1.0f;

    pipelineBuilder._scissor.offset = {0, 0};
    pipelineBuilder._scissor.extent = _windowExtent;

    pipelineBuilder._rasterizer = vkinit::rasterization_state_create_info(VK_POLYGON_MODE_FILL);

    pipelineBuilder._multisampling = vkinit::multisampling_state_create_info();

    pipelineBuilder._colorBlendAttachment = vkinit::color_blend_attachment_state();

    pipelineBuilder._depthStencil = vkinit::depth_stencil_create_info(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);

    DEBUG("before creating the _DiSPipeline");

    // #ff0000
    _fQSPipeline = pipelineBuilder.build_pipeline(_device, _renderPass);
    // #ff0000

    // ----------------------------------------

    VkPipelineLayoutCreateInfo AG_pipeline_layout_info = vkinit::pipeline_layout_create_info();

    VkPushConstantRange AGpush_constant;
	AGpush_constant.offset = 0;
	AGpush_constant.size = sizeof(AGPushConstants);
	AGpush_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	AG_pipeline_layout_info.pPushConstantRanges = &AGpush_constant;
	AG_pipeline_layout_info.pushConstantRangeCount = 1;

    VK_CHECK(vkCreatePipelineLayout(_device, &AG_pipeline_layout_info, nullptr, &_AGPipelineLayout));

    VertexInputDescription AGvertexDescription = Vertex::getAG_vertex_description();

	pipelineBuilder._vertexInputInfo.pVertexAttributeDescriptions = AGvertexDescription.attributes;
	pipelineBuilder._vertexInputInfo.vertexAttributeDescriptionCount = AGvertexDescription.sizeOfAttributes;

	pipelineBuilder._vertexInputInfo.pVertexBindingDescriptions = AGvertexDescription.bindings;
	pipelineBuilder._vertexInputInfo.vertexBindingDescriptionCount = AGvertexDescription.sizeOfBindings;

	pipelineBuilder._shaderStages.clear();

    VkShaderModule AGFragShader;
    if (!load_shader_module("./shaders/DS.frag.spv", &AGFragShader))
    {
        DEBUG("AG frag error");
    }
    else {
        DEBUG("AG frag loaded");
    }

    VkShaderModule AGVertShader;
	if (!load_shader_module("./shaders/DS.vert.spv", &AGVertShader))
	{
		DEBUG("Error when building the AG vertex shader module");
	}
	else {
		DEBUG("AG vertex shader successfully loaded");
	}

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, AGVertShader));

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, AGFragShader));

    pipelineBuilder._pipelineLayout = _AGPipelineLayout;

    DEBUG("PipelineBuilder AG");

    pipelineBuilder._vertexInputInfo = vkinit::vertex_input_state_create_info();

    pipelineBuilder._inputAssembly = vkinit::input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    pipelineBuilder._viewport.x = 0.0f;
    pipelineBuilder._viewport.y = 0.0f;
    pipelineBuilder._viewport.width = (float)_windowExtent.width;
    pipelineBuilder._viewport.height = (float)_windowExtent.height;
    pipelineBuilder._viewport.minDepth = 0.0f;
    pipelineBuilder._viewport.maxDepth = 1.0f;

    pipelineBuilder._scissor.offset = {0, 0};
    pipelineBuilder._scissor.extent = _windowExtent;

    pipelineBuilder._rasterizer = vkinit::rasterization_state_create_info(VK_POLYGON_MODE_FILL);

    pipelineBuilder._multisampling = vkinit::multisampling_state_create_info();

    pipelineBuilder._colorBlendAttachment = vkinit::color_blend_attachment_state();

    pipelineBuilder._depthStencil = vkinit::depth_stencil_create_info(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);

	_AGPipeline = pipelineBuilder.build_pipeline(_device, _renderPass);

    // //--------------------------------------
    // //--------------------------------------
    // //--------------------------------------
    // //--------------------------------------
    // //--------------------------------------

    VkPipelineLayoutCreateInfo mesh_pipeline_layout_info = vkinit::pipeline_layout_create_info();

    VkPushConstantRange push_constant;
    push_constant.offset = 0;
    push_constant.size = sizeof(MeshPushConstants);
    push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    mesh_pipeline_layout_info.pPushConstantRanges = &push_constant;
    mesh_pipeline_layout_info.pushConstantRangeCount = 1;

    mesh_pipeline_layout_info.setLayoutCount = 1;
    mesh_pipeline_layout_info.pSetLayouts = &_globalSetLayout;

    VK_CHECK(vkCreatePipelineLayout(_device, &mesh_pipeline_layout_info, nullptr, &_meshPipelineLayout));

    VertexInputDescription vertexDescription = Vertex::get_vertex_description();

	pipelineBuilder._vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes;
	pipelineBuilder._vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.sizeOfAttributes;

	pipelineBuilder._vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings;
	pipelineBuilder._vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.sizeOfBindings;

	pipelineBuilder._shaderStages.clear();

    VkShaderModule meshVertShader;
	if (!load_shader_module("./shaders/PMtriangle.vert.spv", &meshVertShader))
	{
		DEBUG("Error when building the triangle vertex shader module");
	}
	else {
		DEBUG("Red Triangle vertex shader successfully loaded");
	}

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, meshVertShader));

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, pinkTriangleFragShader));

    pipelineBuilder._pipelineLayout = _meshPipelineLayout;

    DEBUG("PipelineBuilder red triangle");

    pipelineBuilder._vertexInputInfo = vkinit::vertex_input_state_create_info();

    pipelineBuilder._inputAssembly = vkinit::input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    pipelineBuilder._viewport.x = 0.0f;
    pipelineBuilder._viewport.y = 0.0f;
    pipelineBuilder._viewport.width = (float)_windowExtent.width;
    pipelineBuilder._viewport.height = (float)_windowExtent.height;
    pipelineBuilder._viewport.minDepth = 0.0f;
    pipelineBuilder._viewport.maxDepth = 1.0f;

    pipelineBuilder._scissor.offset = {0, 0};
    pipelineBuilder._scissor.extent = _windowExtent;

    pipelineBuilder._rasterizer = vkinit::rasterization_state_create_info(VK_POLYGON_MODE_FILL);

    pipelineBuilder._multisampling = vkinit::multisampling_state_create_info();

    pipelineBuilder._colorBlendAttachment = vkinit::color_blend_attachment_state();

    pipelineBuilder._depthStencil = vkinit::depth_stencil_create_info(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);

	_meshPipeline = pipelineBuilder.build_pipeline(_device, _renderPass);

    create_material(_meshPipeline, _meshPipelineLayout, "defaultmesh");

    vkDestroyShaderModule(_device, pinkTriangleVertexShader, nullptr);
    vkDestroyShaderModule(_device, pinkTriangleFragShader, nullptr);
    vkDestroyShaderModule(_device, triangleVertexShader, nullptr);
    vkDestroyShaderModule(_device, triangleFragShader, nullptr);
    vkDestroyShaderModule(_device, meshVertShader, nullptr);

    _mainDeletionQueue.push_function([=]() {
    
        vkDestroyPipeline(_device, _pinkTrianglePipeline, nullptr);
        vkDestroyPipeline(_device, _trianglePipeline, nullptr);
        vkDestroyPipeline(_device, _meshPipeline, nullptr);
        vkDestroyPipeline(_device, _AGPipeline, nullptr);

        vkDestroyPipelineLayout(_device, _meshPipelineLayout, nullptr);
        vkDestroyPipelineLayout(_device, _trianglePipelineLayout, nullptr);
        vkDestroyPipelineLayout(_device, _AGPipelineLayout, nullptr);
    });
}

void load_meshes()
{
    DEBUG("started loading meshes");
    // _triangleMesh._vertices.resize(3);

	// _triangleMesh._vertices[0].position = { 1.f,1.f, 0.5f };
	// _triangleMesh._vertices[1].position = { -1.f,1.f, 0.5f };
	// _triangleMesh._vertices[2].position = { 0.f,-1.f, 0.5f };

	// _triangleMesh._vertices[0].color = { 0.f,1.f, 0.0f };
	// _triangleMesh._vertices[1].color = { 0.f,1.f, 0.0f }; 
	// _triangleMesh._vertices[2].color = { 0.f,1.f, 0.0f };




    // upload_mesh(_triangleMesh);

	// _meshes["triangle"] = _triangleMesh;
}



// VMA_CALL_PRE VkResult VMA_CALL_POST MKTVulkanAllocDebugFunction(
//     VmaAllocator allocator,
//     const VkBufferCreateInfo* pBufferCreateInfo,
//     const VmaAllocationCreateInfo* pAllocationCreateInfo,
//     VkBuffer* pBuffer,
//     VmaAllocation* pAllocation,
//     VmaAllocationInfo* pAllocationInfo)
// {
//     DEBUG("Started allocation");
//     VMA_ASSERT(allocator && pBufferCreateInfo && pAllocationCreateInfo && pBuffer && pAllocation);

//     if(pBufferCreateInfo->size == 0)
//     {
//         return VK_ERROR_INITIALIZATION_FAILED;
//     }
//     if((pBufferCreateInfo->usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_COPY) != 0 &&
//         !allocator->m_UseKhrBufferDeviceAddress)
//     {
//         VMA_ASSERT(0 && "Creating a buffer with VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT is not valid if VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT was not used.");
//         return VK_ERROR_INITIALIZATION_FAILED;
//     }

//     VMA_DEBUG_LOG("vmaCreateBuffer");
//     DEBUG("After VMA_DEBUG_LOG");

//     VMA_DEBUG_GLOBAL_MUTEX_LOCK

//     *pBuffer = VK_NULL_HANDLE;
//     *pAllocation = VK_NULL_HANDLE;

//     DEBUG("testing ground");

//     printf("\nthis is just chad %x\n",*allocator->GetVulkanFunctions().vkCreateBuffer);

//     printf("\n0. %x",allocator->m_hDevice);
//     printf("\n1. %x",pBufferCreateInfo);
//     printf("\n2. %x",allocator->GetAllocationCallbacks());
//     printf("\n3. %x",pBuffer);

//     //this little cheeser is causing the problems (VKAPI_PTR *PFN_vkCreateBuffer)(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)

//     printf("\nthis is just a problem %x\n",(*allocator->GetVulkanFunctions().vkCreateBuffer)(
//         allocator->m_hDevice,
//         pBufferCreateInfo,
//         allocator->GetAllocationCallbacks(),
//         pBuffer));

//     DEBUG("Before res assigment");

//     // 1. Create VkBuffer.
//     VkResult res = (*allocator->GetVulkanFunctions().vkCreateBuffer)(
//         allocator->m_hDevice,
//         pBufferCreateInfo,
//         allocator->GetAllocationCallbacks(),
//         pBuffer);

//     DEBUG("Before Main \"IF\"");
//     if(res >= 0)
//     {
//         // 2. vkGetBufferMemoryRequirements.
//         VkMemoryRequirements vkMemReq = {};
//         bool requiresDedicatedAllocation = false;
//         bool prefersDedicatedAllocation  = false;
//         allocator->GetBufferMemoryRequirements(*pBuffer, vkMemReq,
//             requiresDedicatedAllocation, prefersDedicatedAllocation);

//         // 3. Allocate memory using allocator.
//         res = allocator->AllocateMemory(
//             vkMemReq,
//             requiresDedicatedAllocation,
//             prefersDedicatedAllocation,
//             *pBuffer, // dedicatedBuffer
//             pBufferCreateInfo->usage, // dedicatedBufferUsage
//             VK_NULL_HANDLE, // dedicatedImage
//             *pAllocationCreateInfo,
//             VMA_SUBALLOCATION_TYPE_BUFFER,
//             1, // allocationCount
//             pAllocation);
//             DEBUG("MemoryAllocation");

//         if(res >= 0)
//         {
//             // 3. Bind buffer with memory.
//             if((pAllocationCreateInfo->flags & VMA_ALLOCATION_CREATE_DONT_BIND_BIT) == 0)
//             {
//                 res = allocator->BindBufferMemory(*pAllocation, 0, *pBuffer, VMA_NULL);
//             }
//             if(res >= 0)
//             {
//                 // All steps succeeded.
//                 #if VMA_STATS_STRING_ENABLED
//                     (*pAllocation)->InitBufferImageUsage(pBufferCreateInfo->usage);
//                 #endif
//                 if(pAllocationInfo != VMA_NULL)
//                 {
//                     allocator->GetAllocationInfo(*pAllocation, pAllocationInfo);
//                 }

//                 return VK_SUCCESS;
//             }
//             allocator->FreeMemory(
//                 1, // allocationCount
//                 pAllocation);
//             *pAllocation = VK_NULL_HANDLE;
//             (*allocator->GetVulkanFunctions().vkDestroyBuffer)(allocator->m_hDevice, *pBuffer, allocator->GetAllocationCallbacks());
//             *pBuffer = VK_NULL_HANDLE;
//             return res;
//         }
//         (*allocator->GetVulkanFunctions().vkDestroyBuffer)(allocator->m_hDevice, *pBuffer, allocator->GetAllocationCallbacks());
//         *pBuffer = VK_NULL_HANDLE;
//         return res;
//     }
//             DEBUG("end return");
//     return res;
// }

void load_AG()
{
    DEBUG("started loading AG");

    const char * MKTAGName = "./DataFiles/blackLine.MKTAG";
    _HexAg = arrayGraphicsReader(MKTAGName);
    _TAGA.push_back(_HexAg);
    upload_AG(_TAGA[_TAGA.size()-1]);
    printf("\nDEBUG: %f %f %f %f %f %f\n",_HexAg._vertices[0].position[0],_HexAg._vertices[0].position[1],_HexAg._vertices[0].position[2],_HexAg._vertices[1].position[0],_HexAg._vertices[1].position[1],_HexAg._vertices[1].position[2]);

    // MKTAG AG;
    // AGPushConstants AGPC;
    // char isVisible;

    _defaultPictureRectangle._vertices = (MKTAPiC*)malloc(4*sizeof(MKTAPiC));

    // VkBuffer _depthImage;
    // VkDeviceMemory _depthImage;

    _defaultPictureRectangle._vertices[0] = {{-0.5f, -0.5f,0.0},{0.0,0.0,0.0},{0.0,0.0}};
    _defaultPictureRectangle._vertices[1] = {{0.5f, -0.5f,0.0},{0.0,0.0,0.0},{1.0,0.0}};
    _defaultPictureRectangle._vertices[2] = {{0.5f, 0.5f,0.0},{0.0,0.0,0.0},{0.0,1.0}};
    _defaultPictureRectangle._vertices[3] = {{-0.5f, 0.5f,0.0},{0.0,0.0,0.0},{1.0,1.0}};
    _defaultPictureRectangle._sizeOfVertices = 4;

    _defaultPictureRectangle.indices = (unsigned int *)malloc(sizeof(unsigned int *)*6);
    _defaultPictureRectangle.sizeOfIndices = 6;

    _defaultPictureRectangle.indices[0] = 0;
    _defaultPictureRectangle.indices[1] = 1;
    _defaultPictureRectangle.indices[2] = 2;
    _defaultPictureRectangle.indices[3] = 2;
    _defaultPictureRectangle.indices[4] = 3;
    _defaultPictureRectangle.indices[5] = 0;

    createIndexBuffer(_defaultPictureRectangle.indices,_defaultPictureRectangle.sizeOfIndices,&_defaultPictureRectangle.indexBuffer,&_defaultPictureRectangle.indexBufferMemory);

    const size_t bufferSize = _defaultPictureRectangle._sizeOfVertices * sizeof(MKTAPiC);
    printf("BS:%d %d\n",bufferSize, sizeof(MKTAPiC));
	VkBufferCreateInfo stagingBufferInfo = {};
	stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferInfo.pNext = nullptr;
	stagingBufferInfo.size = bufferSize;
	stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    DEBUG("before creating a buffer");

    createVertexBuffer(_defaultPictureRectangle._sizeOfVertices * sizeof(MKTAPiC),_defaultPictureRectangle._vertices, &_defaultPictureRectangle._vertexBuffer, &_defaultPictureRectangle._vertexBufferMemory);
}

void loadMenuAG()
{
    const char fileNames[19] = "./lib/icons/.MKTAG";

    CBT = 0.0;

    FILE *MKTFILE;
    MKTFILE = fopen( "./lib/libraryList.MKTI", "rb" );
    char *charArray;
    long sizeOfFile;
    if( MKTFILE != NULL )
    {
        fseek(MKTFILE, 0L, SEEK_END);
        sizeOfFile = ftell(MKTFILE);
        rewind(MKTFILE);
        charArray = (char*)malloc(sizeOfFile);
        sizeOfFile = fread( charArray,1, sizeOfFile, MKTFILE );
        fclose( MKTFILE );
    } else {
        // error file couldn't openprintf
    }
    int moduleNumber = 0;
    DEBUG("Read the Icon AG2");
    for(int i = 1; i < sizeOfFile;i++)
    {
        if(charArray[i-1] == '\n' && charArray[i] != '{')
        {
        DEBUG("Read the Icon AG3");
            char * MKTAGName;
            size_t sizeOfName = 0;
        DEBUG("Read the Icon AG4");
        for(0; charArray[i+sizeOfName+1] != '\n' && i+sizeOfName < sizeOfFile;sizeOfName++)0;
        DEBUG("Read the Icon AG5");
            MKTAGName = (char*)malloc(sizeOfName+19);
        DEBUG("Read the Icon AG6");
            for(int a = 0; a < sizeOfName;a++)
                MKTAGName[a+12] = charArray[a+i];
            for(int a = 0; a < 12;a++)
                MKTAGName[a] = fileNames[a];
            for(int a = 0; a < 7;a++)
                MKTAGName[sizeOfName+12+a] = fileNames[a+12];
             DEBUG("Read the Icon AG7");
            Modules[moduleNumber].icon.AG = arrayGraphicsReader(MKTAGName);
            // for(int a = 0; a < 3;a++)
            // Modules[moduleNumber].icon.AG._vertices[a].color = {1.0,1.0,1.0};
            AGPushConstants DEFAULTconstants;
            DEFAULTconstants.colourModification = {1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f}; // black
            float ratio = _windowExtent.width;
            ratio /= _windowExtent.height;
             DEBUG("Read the Icon AG8");
            DEFAULTconstants.transformation = {0.05/ratio
            ,0.0,0.0,0.0,0.0,
                                        0.05
            ,0.0,0.0,0.0,0.0,
                                        0.0
            ,0.0,0.0,0.0,0.0,
                                        0.0
            };

            DEFAULTconstants.movement = {0.0-((_windowExtent.width*0.98)/_windowExtent.width),0.0-((_windowExtent.height*0.95)/_windowExtent.height),0.0};
            Modules[moduleNumber].icon.isVisible = 1;
            Modules[moduleNumber].icon.AGPC = DEFAULTconstants;
            upload_AG(Modules[moduleNumber].icon.AG);
            moduleNumber++;
            free(MKTAGName);
        }
    }
    if(moduleNumber != sizeOfModules)
        0; // error, list of modules was changed whilst loading AGs
    DEBUG("Read the Icon AG");
    DEBUG("Loaded an Icon AG");
}
    

void init_scene()
{
    // RenderObject DUCK;
    // AGMaterial = {_AGPipeline,_AGPipelineLayout};
	// DUCK.mesh = get_mesh("DUCK");
	// DUCK.material = get_material("defaultmesh");
	// DUCK.transformMatrix = glm::mat4{ 1.0f };
	// _renderables.push_back(DUCK);

    AGPushConstants AGconstants;

    AGconstants.transformation = {2.0,0.0,0.0,0.0, // i screwed something up with the math, and now everything is 2 times smaller, :D
                                0.0,2.0,0.0,0.0,
                                0.0,0.0,2.0,0.0,
                                0.0,0.0,0.0,1.0};

    

    AGconstants.colourModification = {1.0,0.0,0.0,0.0,
                                    0.0,1.0,0.0,0.0,
                                    0.0,0.0,1.0,0.0,
                                    0.0,0.0,0.0,1.0};

    AGconstants.movement = {0.0,0.0,0.0};

    // const char * defaultName = "defaultName";
    AGPushConstants BLconstants;
    ratio = _windowExtent.width;
    ratio /= _windowExtent.height;
    BLconstants.transformation = {0.05/ratio
            ,0.0,0.0,0.0,0.0,
                                        0.004
            ,0.0,0.0,0.0,0.0,
                                        0.0
            ,0.0,0.0,0.0,0.0,
                                        0.0};

            

    BLconstants.colourModification = {1.0,0.0,0.0,0.0,
                                    0.0,1.0,0.0,0.0,
                                    0.0,0.0,1.0,0.0,
                                    0.0,0.0,0.0,1.0};

    BLconstants.movement = {0.0-((_windowExtent.width*0.98)/_windowExtent.width),0.0-((_windowExtent.height*0.95)/_windowExtent.height),0.0};;
    

    for(int i = 0; i < _TAGA.size();i++)
    {
        _AGA.push_back({_TAGA[i],AGconstants,1});//,&AGMaterial
    }
    _AGA[0].AGPC = BLconstants;
    _AGA[0].isVisible = 0;
	for (int x = -20; x <= 20; x++) {
		for (int y = -20; y <= 20; y++) {

			RenderObject tri;
			tri.mesh = get_mesh("triangle");
			tri.material = get_material("defaultmesh");
			glm::mat4 translation = glm::translate(glm::mat4{ 1.0 }, glm::vec3(x, 0, y));
			glm::mat4 scale = glm::scale(glm::mat4{ 1.0 }, glm::vec3(0.2, 0.2, 0.2));
			tri.transformMatrix = translation * scale;

			_renderables.push_back(tri);
		}
	}
    DEBUG("init: scene");
}

void init_VentumEngineVariables()
{
    setupRenderArray(&_render);

    CBTStatus = 4;

    modesEnabled = 0;//MKTDRAWPIC; // |MKTDRAWAG|MKTDRAWMENU; 14;

    DEBUG("VentumEngineVariables loaded");

    return;
}

void CreatePictureDescriptors(vPic * pPic, size_t index)
{
    _currentPicturesDescriptorSize++;
    _currentPicturesDescriptor = (VkDescriptorSet*)realloc(_currentPicturesDescriptor,sizeof(VkDescriptorSet)*_currentPicturesDescriptorSize);
    VkDescriptorSetAllocateInfo allocInfo ={};
    allocInfo.pNext = nullptr;
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = _descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &_picturesSetLayout;
    VK_CHECK(vkAllocateDescriptorSets(_device, &allocInfo, &_currentPicturesDescriptor[index]));

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = pPic[index].textureImageView;
    imageInfo.sampler = _textureSampler;

    VkWriteDescriptorSet setWrite = {};
    setWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    setWrite.dstSet = _currentPicturesDescriptor[index];
    setWrite.dstBinding = 0;
    setWrite.dstArrayElement = 0;
    setWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    setWrite.descriptorCount = 1;
    setWrite.pImageInfo = &imageInfo;
    setWrite.pNext = nullptr;
    
    vkUpdateDescriptorSets(_device, 1, &setWrite, 0, nullptr);

    return;
}

void init_descriptors()
{
    _currentPicturesDescriptorSize = 0;
    _currentPicturesDescriptor = (VkDescriptorSet*)malloc(sizeof(VkDescriptorSet)*3);
    
    VkDescriptorSetLayoutBinding camBufferBinding = {};
	camBufferBinding.binding = 0;
	camBufferBinding.descriptorCount = 1;
	camBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	camBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutCreateInfo setInfo = {};
	setInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setInfo.pNext = nullptr;

	setInfo.bindingCount = 1;
	setInfo.flags = 0;
    setInfo.pBindings = &camBufferBinding;

	vkCreateDescriptorSetLayout(_device, &setInfo, nullptr, &_globalSetLayout);

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 0;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo setinfo = {};
	setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setinfo.pNext = nullptr;

	setinfo.bindingCount = 1;
	setinfo.flags = 0;
    setinfo.pBindings = &samplerLayoutBinding;

    vkCreateDescriptorSetLayout(_device, &setinfo, nullptr, &_picturesSetLayout);

    VkDescriptorPoolSize size = {};
    size.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    size.descriptorCount = 10; // to change

    VkDescriptorPoolCreateInfo pP_info = {};
	pP_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pP_info.flags = 0;
	pP_info.maxSets = 10; // to change
	pP_info.poolSizeCount = 1;
	pP_info.pPoolSizes = &size;
    
	VK_CHECK(vkCreateDescriptorPool(_device, &pP_info, nullptr, &_picturesDescriptorPool));

    std::vector<VkDescriptorPoolSize> sizes =
	{
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 },{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,10}
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = 0;
	pool_info.maxSets = 10;
	pool_info.poolSizeCount = (uint32_t)sizes.size();
	pool_info.pPoolSizes = sizes.data();

	vkCreateDescriptorPool(_device, &pool_info, nullptr, &_descriptorPool);

    for (int i = 0; i < FRAME_OVERLAP; i++)
	{
        createBuffer(sizeof(GPUCameraData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &_frames[i]._cameraBuffer, &_frames[i]._cameraBufferMemory);

		VkDescriptorSetAllocateInfo allocInfo ={};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &_globalSetLayout;

		vkAllocateDescriptorSets(_device, &allocInfo, &_frames[i].globalDescriptor);

        VkDescriptorBufferInfo binfo;
		binfo.buffer = _frames[i]._cameraBuffer;
		binfo.offset = 0;
		binfo.range = sizeof(GPUCameraData);

		VkWriteDescriptorSet setWrite;
		setWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        setWrite.dstSet = _frames[i].globalDescriptor;
        setWrite.dstBinding = 0;
        setWrite.dstArrayElement = 0;
        setWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        setWrite.descriptorCount = 1;
        setWrite.pBufferInfo = &binfo;
        setWrite.pNext = nullptr;
        vkUpdateDescriptorSets(_device, 1, &setWrite, 0, nullptr);
	}

    CreatePictureDescriptors(_pictures.data(),0);

    _mainDeletionQueue.push_function([&]() {
		vkDestroyDescriptorSetLayout(_device, _globalSetLayout, nullptr);
		vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
	});

	for (int i = 0; i < FRAME_OVERLAP; i++)
	{
		_mainDeletionQueue.push_function([&]() {
			// error no cleanup
		});
	}

    imagesLoaded = 1;

    // _pictures[0]._currentPool = _descriptorPool;
    // _pictures[0]._currentDescriptor = (VkDescriptorSet*)malloc(sizeof(VkDescriptorSet)*FRAME_OVERLAP);
    // for(int i = 0; i < FRAME_OVERLAP;i++)
        // _pictures[0]._currentDescriptor[i] = _frames[i].globalDescriptor;
}

void updateSpecificPictureDescriptor(vPic * Picture, size_t pictureNumber)
{
    FrameData * tempFrameData = (FrameData*)malloc(sizeof(FrameData)*FRAME_OVERLAP);

    VkDescriptorSetLayoutBinding camBufferBinding = {};
	camBufferBinding.binding = 0;
	camBufferBinding.descriptorCount = 1;
	camBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	camBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;


	VkDescriptorSetLayoutCreateInfo setinfo = {};
	setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setinfo.pNext = nullptr;

	setinfo.bindingCount = 2;
	setinfo.flags = 0;
	VkDescriptorSetLayoutBinding* tempArray = (VkDescriptorSetLayoutBinding*)malloc(sizeof(VkDescriptorSetLayoutBinding)*setinfo.bindingCount);
    tempArray[0] = camBufferBinding;
    tempArray[1] = samplerLayoutBinding;
    setinfo.pBindings = tempArray;

	vkCreateDescriptorSetLayout(_device, &setinfo, nullptr, &_globalSetLayout);

    std::vector<VkDescriptorPoolSize> sizes =
	{
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 },{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,10}
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = 0;
	pool_info.maxSets = 1;
	pool_info.poolSizeCount = (uint32_t)sizes.size();
	pool_info.pPoolSizes = sizes.data();

	// vkCreateDescriptorPool(_device, &pool_info, nullptr, &Picture[pictureNumber]._currentPool);

    for (int i = 0; i < FRAME_OVERLAP; i++)
	{
        createBuffer(sizeof(GPUCameraData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &tempFrameData[i]._cameraBuffer, &tempFrameData[i]._cameraBufferMemory);


		VkDescriptorSetAllocateInfo allocInfo ={};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &_globalSetLayout;

		vkAllocateDescriptorSets(_device, &allocInfo, &tempFrameData[i].globalDescriptor);

        VkDescriptorBufferInfo binfo;
		binfo.buffer = tempFrameData[i]._cameraBuffer;
		binfo.offset = 0;
		binfo.range = sizeof(GPUCameraData);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = _pictures[pictureNumber].textureImageView;
        imageInfo.sampler = _textureSampler;

		VkWriteDescriptorSet * setWrite = (VkWriteDescriptorSet*)malloc(sizeof(VkWriteDescriptorSet)*2);
		setWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        setWrite[0].dstSet = tempFrameData[i].globalDescriptor;
        setWrite[0].dstBinding = 0;
        setWrite[0].dstArrayElement = 0;
        setWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        setWrite[0].descriptorCount = 1;
        setWrite[0].pBufferInfo = &binfo;
        setWrite[0].pNext = nullptr;

        setWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        setWrite[1].dstSet = tempFrameData[i].globalDescriptor;
        setWrite[1].dstBinding = 1;
        setWrite[1].dstArrayElement = 0;
        setWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        setWrite[1].descriptorCount = 1;
        setWrite[1].pImageInfo = &imageInfo;
        setWrite[1].pNext = nullptr;


		vkUpdateDescriptorSets(_device, 2, setWrite, 0, nullptr);
	}

	for (int i = 0; i < FRAME_OVERLAP; i++)
	{
		_mainDeletionQueue.push_function([&]() {
			// error again
		});
	}

    // _pictures[pictureNumber]._currentPool = _descriptorPool;
    // _pictures[pictureNumber]._currentDescriptor = (VkDescriptorSet*)malloc(sizeof(VkDescriptorSet)*FRAME_OVERLAP);
    // for(int i = 0; i < FRAME_OVERLAP;i++)
        // _pictures[pictureNumber]._currentDescriptor[i] = tempFrameData[i].globalDescriptor;

// printf("\nTHIS WILL BE FUN: %x %d",_pictures[1]._currentDescriptor,*(int*)(_pictures[1]._currentDescriptor));
    free(tempFrameData);

    // VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    // samplerLayoutBinding.binding = 0;
    // samplerLayoutBinding.descriptorCount = 1;
    // samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // samplerLayoutBinding.pImmutableSamplers = nullptr;
    // samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;


	// VkDescriptorSetLayoutCreateInfo setinfo = {};
	// setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	// setinfo.pNext = nullptr;

	// setinfo.bindingCount = 1;
	// setinfo.flags = 0;
    // setinfo.pBindings = &samplerLayoutBinding;

    // VK_CHECK(vkCreateDescriptorSetLayout(_device, &setinfo, nullptr, &Picture[pictureNumber]._currentSetLayout));

    // std::vector<VkDescriptorPoolSize> sizes =
	// {
	// 	{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,10}
	// };

	// VkDescriptorPoolCreateInfo pool_info = {};
	// pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	// pool_info.flags = 0;
	// pool_info.maxSets = FRAME_OVERLAP;
	// pool_info.poolSizeCount = (uint32_t)sizes.size();
	// pool_info.pPoolSizes = sizes.data();

    // Picture[pictureNumber]._currentDescriptor = (VkDescriptorSet*)malloc(sizeof(VkDescriptorSet)*FRAME_OVERLAP);

	// VK_CHECK(vkCreateDescriptorPool(_device, &pool_info, nullptr, &Picture[pictureNumber]._currentPool));

    // _pictures[0]._currentDescriptor = (VkDescriptorSet*)malloc(sizeof(VkDescriptorSet)*FRAME_OVERLAP);
    // for(int i = 0; i < FRAME_OVERLAP;i++)
    // {
    // DEBUG("error check 2");
    //     VkDescriptorSetAllocateInfo allocInfo ={};
    //     allocInfo.pNext = nullptr;
    //     allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    //     allocInfo.descriptorPool = Picture[pictureNumber]._currentPool;
    //     allocInfo.descriptorSetCount = 1;
    //     allocInfo.pSetLayouts = &Picture[pictureNumber]._currentSetLayout;

    //     VK_CHECK(vkAllocateDescriptorSets(_device, &allocInfo, &Picture[pictureNumber]._currentDescriptor[i]));

    //     VkDescriptorImageInfo imageInfo{};
    //     imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //     imageInfo.imageView = _pictures[pictureNumber].textureImageView;
    //     imageInfo.sampler = _textureSampler;

    //     VkWriteDescriptorSet * setWrite = (VkWriteDescriptorSet*)malloc(sizeof(VkWriteDescriptorSet));
    //     setWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    //     setWrite[0].dstSet = Picture[pictureNumber]._currentDescriptor[i];
    //     setWrite[0].dstBinding = 0;
    //     setWrite[0].dstArrayElement = 0;
    //     setWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //     setWrite[0].descriptorCount = 1;
    //     setWrite[0].pImageInfo = &imageInfo;
    //     setWrite[0].pNext = nullptr;

    // DEBUG("error check 3");
    //     vkUpdateDescriptorSets(_device, 1, setWrite, 0, nullptr);
    // }


    // _mainDeletionQueue.push_function([&]() {
	// 	vkDestroyDescriptorSetLayout(_device, _globalSetLayout, nullptr);
	// 	vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
	// });
}


void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function)
{
	VkCommandBuffer cmd = _uploadContext._commandBuffer;

	VkCommandBufferBeginInfo cmdBeginInfo = vkinit::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

	function(cmd);

	VK_CHECK(vkEndCommandBuffer(cmd));

	VkSubmitInfo submit = vkinit::submit_info(&cmd);


	VK_CHECK(vkQueueSubmit(_graphicsQueue, 1, &submit, _uploadContext._uploadFence));

	vkWaitForFences(_device, 1, &_uploadContext._uploadFence, true, 9999999999);
	vkResetFences(_device, 1, &_uploadContext._uploadFence);
	vkResetCommandPool(_device, _uploadContext._commandPool, 0);
}


//----------------------------------------------------------------------------------------------------------------------------------------

VkPipeline PipelineBuilder::build_pipeline(VkDevice device, VkRenderPass pass)
{
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;

    viewportState.viewportCount = 1;
    viewportState.pViewports = &_viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &_scissor;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.pNext = nullptr;

    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &_colorBlendAttachment;

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;

    pipelineInfo.stageCount = _shaderStages.size();
    pipelineInfo.pStages = _shaderStages.data();
    pipelineInfo.pVertexInputState = &_vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &_inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &_rasterizer;
    pipelineInfo.pMultisampleState = &_multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = _pipelineLayout;
    pipelineInfo.renderPass = pass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.pDepthStencilState = &_depthStencil;

    VkPipeline newPipeline;
    if (vkCreateGraphicsPipelines(
        device, VK_NULL_HANDLE, 1, &pipelineInfo , nullptr, &newPipeline) != VK_SUCCESS) {
            std::cout << "failed to create pipeline\n";
            return VK_NULL_HANDLE;
        }
        else {
            return newPipeline;
        }
}

void upload_AG(MKTAG& AG)
{
    DEBUG("started loading in AG");
    printf("size: %d %d %d %d\n",AG._sizeOfVertices, sizeof(glm::vec3), sizeof(glm::vec3)*2*4 ,AG._sizeOfVertices * sizeof(MKTAGA));
    for(int i = 0; i < 4; i++)
        for(int a = 0; a < 3; a++)
            printf("_vertices[%d][%d] %f %f\n",i,a,AG._vertices[i].position[a], AG._vertices[i].color[a]);
    createVertexBuffer(AG._sizeOfVertices * sizeof(MKTAGA), AG._vertices, &AG._vertexBuffer, &AG._vertexBufferMemory);
    _mainDeletionQueue.push_function([=]() {
		// error cleanup for the buffer
	});
    DEBUG("before unmapping");
    createIndexBuffer(AG.indices,AG.sizeOfIndices,&AG.indexBuffer,&AG.indexBufferMemory);
}

void init_createTextureSampler()
{
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(_chosenGPU, &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(_chosenGPU, &supportedFeatures);
    samplerInfo.anisotropyEnable = VK_FALSE; // supportedFeatures.samplerAnisotropy?VK_TRUE:
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(_device, &samplerInfo, nullptr, &_textureSampler) != VK_SUCCESS) {
        // errorHandling
    }
    _mainDeletionQueue.push_function([=]() {
        vkDestroySampler(_device, _textureSampler, nullptr);
    });
}

void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(
    commandBuffer,
    buffer,
    image,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    1,
    &region
    );

    endSingleTimeCommands(commandBuffer);
}

void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    endSingleTimeCommands(commandBuffer);
}

VkCommandBuffer beginSingleTimeCommands() {


    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = get_current_frame()._commandPool; // this is a bad idea
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_graphicsQueue);

    vkFreeCommandBuffers(_device, get_current_frame()._commandPool, 1, &commandBuffer);
}

void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK(vkCreateImage(_device, &imageInfo, nullptr, &image));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    VK_CHECK(vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory));

    vkBindImageMemory(_device, image, imageMemory, 0);
}

// void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
//     VkBufferCreateInfo bufferInfo{};
//     bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//     bufferInfo.size = size;
//     bufferInfo.usage = usage;
//     bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

//     VK_CHECK(vkCreateBuffer(_device, &bufferInfo, nullptr, &buffer));

//         DEBUG("created a buffer");
//     VkMemoryRequirements memRequirements;
//     vkGetBufferMemoryRequirements(_device, buffer, &memRequirements);

//     VkMemoryAllocateInfo allocInfo{};
//     allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//     allocInfo.allocationSize = memRequirements.size;
//     allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

//     if(allocInfo.memoryTypeIndex == -1)
//         DEBUG("and we have a problem on our hands");

//     VK_CHECK(vkAllocateMemory(_device, &allocInfo, nullptr, &bufferMemory));
//         DEBUG("allocated memory");

//     vkBindBufferMemory(_device, buffer, bufferMemory, 0);
//         DEBUG("bound buffer memory");
// }

void logoLoadingFunctionThatIsVeryImportant()
{
    MKTPic data;
    FILE *MKTFILE = fopen("./DataFiles/logo.MKTP", "rb" );
	char *list;
	fseek(MKTFILE,0L,SEEK_END);
	size_t sizeOfFile = ftell(MKTFILE);
	rewind(MKTFILE);
	list = (char*)malloc(sizeOfFile);
	fread( list,1, sizeOfFile, MKTFILE );

    data.width = *(size_t*)list;
    data.pixels = (MKTPix*)malloc(sizeOfFile-sizeof(size_t));

    for(int i = 0; i < sizeOfFile-sizeof(size_t);i++)
    {
        if(i%4 == 0)
            data.pixels[(i-(i%4))/4].R = list[i+sizeof(size_t)];
        if(i%4 == 1)
            data.pixels[(i-(i%4))/4].G = list[i+sizeof(size_t)];
        if(i%4 == 2)
            data.pixels[(i-(i%4))/4].B = list[i+sizeof(size_t)];
        if(i%4 == 3)
           data.pixels[(i-(i%4))/4].A = list[i+sizeof(size_t)];
    }
	
    fclose( MKTFILE );

    data.height = (sizeOfFile-sizeof(size_t))/sizeof(MKTPix)/data.width;


    VkDeviceSize imageSize = data.width * data.height * 4;

    // createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    VkBufferCreateInfo stagingBufferInfo = {};
	stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferInfo.pNext = nullptr;
	stagingBufferInfo.size = imageSize;
	stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    // AllocatedBuffer stagingBufferMemory;
    // VmaAllocationCreateInfo vmaallocInfo = {};
	// vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY; // VMA_MEMORY_USAGE_CPU_TO_GPU // VMA_MEMORY_USAGE_AUTO

    

    // createVertexBuffer(imageSize, data.pixels, VkBuffer * _vertexBuffer, VkDeviceMemory * _vertexBufferMemory);

	// VK_CHECK(vmaCreateBuffer(_allocator, &stagingBufferInfo, &vmaallocInfo,
		// &stagingBufferMemory._buffer,
		// &stagingBufferMemory._allocation,
		// nullptr));

    DEBUG("created buffer");
    // createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer * buffer, VkDeviceMemory * bufferMemory);
    // vmaMapMemory(_allocator, stagingBufferMemory._allocation, &data_ptr);
	// memcpy(data_ptr, data.pixels, imageSize);
	// vmaUnmapMemory(_allocator, stagingBufferMemory._allocation);
VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    void* data_ptr;
    vkMapMemory(_device, stagingBufferMemory, 0, imageSize, 0, &data_ptr);
    memcpy(data_ptr, data.pixels, static_cast<size_t>(imageSize)); // now let us just hope that my made up format will work in this highly delicate system
    vkUnmapMemory(_device, stagingBufferMemory);
    DEBUG("mapped memory");

    

    free(data.pixels); // #ff0000 might cause some minor fatal errors

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(data.width);
    imageInfo.extent.height = static_cast<uint32_t>(data.height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // Optional
    DEBUG("created image info");
    VK_CHECK(vkCreateImage(_device, &imageInfo, nullptr, &textureImage));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_device, textureImage, &memRequirements);
    DEBUG("got image memory requirements");
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    DEBUG("found memory type");

    VK_CHECK(vkAllocateMemory(_device, &allocInfo, nullptr, &textureImageMemory));
    DEBUG("allocated memory");

    vkBindImageMemory(_device, textureImage, textureImageMemory, 0);

///later
    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(data.width), static_cast<uint32_t>(data.height));

    DEBUG("copied buffer to image");
    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(_device, stagingBuffer, nullptr);
    // vkFreeMemory(_device, stagingBufferMemory, nullptr); // #ff0000

    VkImageView textureImageView;

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = textureImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VK_CHECK(vkCreateImageView(_device, &viewInfo, nullptr, &textureImageView));
    DEBUG("created image view");

    _defaultPushConstants.transformation = {2.0,0.0,0.0,0.0,
                                0.0,2.0,0.0,0.0,
                                0.0,0.0,2.0,0.0,
                                0.0,0.0,0.0,1.0};

    _defaultPushConstants.colourModification = {1.0,0.0,0.0,0.0,
                                    0.0,1.0,0.0,0.0,
                                    0.0,0.0,1.0,0.0,
                                    0.0,0.0,0.0,1.0};

    _defaultPushConstants.movement = {0.0,0.0,0.0};


    _pictures.resize(1);
    _pictures[0] = {textureImage,textureImageMemory,textureImageView,_defaultPushConstants,1};

    _mainDeletionQueue.push_function([=]() {
        vkDestroyImageView(_device, _pictures[0].textureImageView, nullptr);
        vkDestroyImage(_device, _pictures[0].textureImage, nullptr);
        vkFreeMemory(_device, _pictures[0].textureImageMemory, nullptr);
    });
    DEBUG("loaded the logo");
    return;
}

int loading_MKTP_image(MKTPic data)
{
    VkDeviceSize imageSize = data.width * data.height * 4;

    // createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    VkBufferCreateInfo stagingBufferInfo = {};
	stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferInfo.pNext = nullptr;
	stagingBufferInfo.size = imageSize;
	stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    void* data_ptr;
    vkMapMemory(_device, stagingBufferMemory, 0, imageSize, 0, &data_ptr);
    memcpy(data_ptr, data.pixels, static_cast<size_t>(imageSize)); // now let us just hope that my made up format will work in this highly delicate system
    vkUnmapMemory(_device, stagingBufferMemory);
    // vkMapMemory(_device, stagingBufferMemory, 0, imageSize, 0, &data_ptr);
    // memcpy(data_ptr, data.pixels, static_cast<size_t>(imageSize)); // now let us just hope that my made up format will work in this highly delicate system
    // vkUnmapMemory(_device, stagingBufferMemory);
    DEBUG("mapped memory");

    

    free(data.pixels); // #ff0000 might cause some minor fatal errors

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(data.width);
    imageInfo.extent.height = static_cast<uint32_t>(data.height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // Optional
    DEBUG("created image info");
    VK_CHECK(vkCreateImage(_device, &imageInfo, nullptr, &textureImage));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_device, textureImage, &memRequirements);
    DEBUG("got image memory requirements");
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    DEBUG("found memory type");

    VK_CHECK(vkAllocateMemory(_device, &allocInfo, nullptr, &textureImageMemory));
    DEBUG("allocated memory");

    vkBindImageMemory(_device, textureImage, textureImageMemory, 0);

///later
    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(data.width), static_cast<uint32_t>(data.height));

    DEBUG("copied buffer to image");
    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(_device, stagingBuffer, nullptr);
    // vkFreeMemory(_device, stagingBufferMemory, nullptr); // #ff0000

    VkImageView textureImageView;

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = textureImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VK_CHECK(vkCreateImageView(_device, &viewInfo, nullptr, &textureImageView));
    DEBUG("created image view");


    _pictures.resize(_pictures.size()+1);
    imagesLoaded++;
    _pictures[_pictures.size()-1] = {textureImage,textureImageMemory,textureImageView,_defaultPushConstants,1};

    _pictures[_pictures.size()-1].PC.transformation = {0.1,0.0,0.0,0.0,
                                    0.0,0.1,0.0,0.0,
                                    0.0,0.0,0.1,0.0,
                                    0.0,0.0,0.0,1.0};


    CreatePictureDescriptors(_pictures.data(),_pictures.size()-1);
    addToRenderArray(&_render,1, _pictures.size()-1, 0);

    _mainDeletionQueue.push_function([=]() {
        vkDestroyImageView(_device, _pictures[_pictures.size()-1].textureImageView, nullptr);
        vkDestroyImage(_device, _pictures[_pictures.size()-1].textureImage, nullptr);
        vkFreeMemory(_device, _pictures[_pictures.size()-1].textureImageMemory, nullptr);
    });
    DEBUG("loaded the logo");
    return _pictures.size()-1;
    // some shananigans
}

void upload_mesh(Mesh& mesh)
{
    createVertexBuffer(mesh._sizeOfVertices * sizeof(Vertex), mesh._vertices, &mesh._vertexBuffer, &mesh._vertexBufferMemory);
}

Material* create_material(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name)
{
	Material mat;
	mat.pipeline = pipeline;
	mat.pipelineLayout = layout;
	_materials[name] = mat;
	return &_materials[name];
}

Material* get_material(const std::string& name)
{
	auto it = _materials.find(name);
	if (it == _materials.end()) {
		return nullptr;
	}
	else {
		return &(*it).second;
	}
}


Mesh* get_mesh(const std::string& name)
{
	auto it = _meshes.find(name);
	if (it == _meshes.end()) {
		return nullptr;
	}
	else {
		return &(*it).second;
	}
}

void MKTDRAW(VkCommandBuffer cmd)
{
    glm::vec3 camPos = { 0.f,-6.f,-10.f };

    glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
    glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
    projection[1][1] *= -1;

    GPUCameraData camData;
    camData.proj = projection;
    camData.view = view;
    camData.viewproj = projection * view;

    char boundDefault = 0;

    void* data;
    vkMapMemory(_device, get_current_frame()._cameraBufferMemory, 0, sizeof(GPUCameraData), 0, &data);
    memcpy(data, &camData, sizeof(GPUCameraData));
    vkUnmapMemory(_device, get_current_frame()._cameraBufferMemory);

    Mesh* lastMesh = nullptr;
    Material* lastMaterial = nullptr;

    RDEBUG("before drawing");

    for(int iLayer = 0; iLayer < _render.size;iLayer++)
    {
        for(int renderObject = 0; renderObject < _render.sizePerArray[iLayer];renderObject++)
        {
            if(modesEnabled&MKTDRAWAG)
                if(_render.arrayPointers[iLayer][renderObject].type == 0)
                {
                    RDEBUG("RRRR DRAW:AG RRRR");
                    int fasterID = _render.arrayPointers[iLayer][renderObject].ID;
                    if(_AGA[fasterID].isVisible)
                    {
                        AGPushConstants constants = _AGA[fasterID].AGPC;
                        MKTAG * object = &_AGA[fasterID].AG;

                        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _AGPipeline);

                        VkDeviceSize offset = 0;

                        vkCmdPushConstants(cmd, _AGPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &constants);

                        vkCmdBindVertexBuffers(cmd, 0, 1, &object->_vertexBuffer, &offset);

                        vkCmdBindIndexBuffer(cmd, _AGA[fasterID].AG.indexBuffer, 0, VK_INDEX_TYPE_UINT16);

                        vkCmdDrawIndexed(cmd, object->sizeOfIndices, 1, 0, 0, 0);
                    }
                }
            if(modesEnabled&MKTDRAWPIC)
                if(_render.arrayPointers[iLayer][renderObject].type == 1)
                {
                    RDEBUG("RRRR DRAW:PIC RRRR");
                    int fasterID = _render.arrayPointers[iLayer][renderObject].ID;
                    if(_pictures[fasterID].isVisible)
                    {
                        AGPushConstants constants = _pictures[fasterID].PC;
                        MKTPiC * object = &_defaultPictureRectangle;

                        VkDeviceSize offset = 0;

                        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _DiSPipeline);

                        vkCmdPushConstants(cmd, _DiSPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &constants);

                        vkCmdBindVertexBuffers(cmd, 0, 1, &object->_vertexBuffer, &offset);//#ff0000 problem

                        vkCmdBindIndexBuffer(cmd, _defaultPictureRectangle.indexBuffer , 0, VK_INDEX_TYPE_UINT16);

                        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _DiSPipelineLayout, 0, 1, &_currentPicturesDescriptor[fasterID], 0, nullptr);

                        printf("idk: %d\n", object->sizeOfIndices);

                        vkCmdDrawIndexed(cmd, object->sizeOfIndices, 1, 0, 0,0);
                        // vkCmdDraw(cmd, object->_sizeOfVertices, 1, 0, 0);
                    }
                }
            if(modesEnabled&MKTDRAW3D)
                if(_render.arrayPointers[iLayer][renderObject].type == 2)
                {
                    RDEBUG("RRRR DRAW:3D RRRR");
                    int fasterID = _render.arrayPointers[iLayer][renderObject].ID;
                    RenderObject& object = _renderables[fasterID];

                    if (object.material != lastMaterial) {

                        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, object.material->pipeline);
                        lastMaterial = object.material;
                        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, object.material->pipelineLayout, 0, 1, &get_current_frame().globalDescriptor, 0, nullptr);
                    }


                    glm::mat4 model = object.transformMatrix;
                    glm::mat4 mesh_matrix = projection * view * model;

                    MeshPushConstants constants;
                    constants.render_matrix = object.transformMatrix;

                    vkCmdPushConstants(cmd, object.material->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants), &constants);

                    if (object.mesh != lastMesh) {
                        VkDeviceSize offset = 0;
                        vkCmdBindVertexBuffers(cmd, 0, 1, &object.mesh->_vertexBuffer, &offset);
                        lastMesh = object.mesh;
                    }
                    vkCmdDraw(cmd, object.mesh->_sizeOfVertices, 1, 0, 0);
                }
            if(modesEnabled&MKTDRAWTEXT)
                if(_render.arrayPointers[iLayer][renderObject].type == 4)
                {
                    RDEBUG("RRRR DRAW:TEXT RRRR");
                    int fasterID = _render.arrayPointers[iLayer][renderObject].ID;
                    if(_TEXT[fasterID].isVisible)
                    {
                        TEXTPushConstant constants = _TEXT[fasterID].pushConstant;
                        MKTAG * object = getFontAG(_TEXT[fasterID]);

                        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _AGPipeline);

                        VkDeviceSize offset = 0;

                        vkCmdPushConstants(cmd, _fQSPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &constants);

                        vkCmdBindVertexBuffers(cmd, 0, 1, &object->_vertexBuffer, &offset);

                        vkCmdBindIndexBuffer(cmd, object->indexBuffer, 0, VK_INDEX_TYPE_UINT16);

                        vkCmdDrawIndexed(cmd, object->sizeOfIndices, 1, 0, 0, 0);
                    }
                }
            
        }
    }

    RDEBUG("after drawing content");

    if(modesEnabled&MKTDRAWMENU)
        drawMenu(cmd,Modules, sizeOfModules);
    RDEBUG("after drawing menu");
    return;
}

void createIndexBuffer(unsigned int * indices,size_t sizeOfIndices,VkBuffer * indexBuffer, VkDeviceMemory * indexBufferMemory)
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * sizeOfIndices;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    void* data;
    vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices, (size_t) bufferSize);
    vkUnmapMemory(_device, stagingBufferMemory);
        DEBUG("created the staging index buffer");
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(stagingBuffer, *indexBuffer, bufferSize);
        DEBUG("created index buffer");

    vkDestroyBuffer(_device, stagingBuffer, nullptr);
    vkFreeMemory(_device, stagingBufferMemory, nullptr);
    // error add cleanup
    return;
}

MKTAG * getFontAG(MKTEXT input)
{
    MKTAG * toReturn = (MKTAG*)malloc(sizeof(MKTAG));
    return toReturn;
}

void drawMenu(VkCommandBuffer cmd,GL * menuStuff, size_t sizeOfMenuStuff)
{
    RDEBUG("RRRR DRAW:MENU RRRR");
	for (int i = 0; i < sizeOfMenuStuff; i++) // make it less than 18 lines per draw
	{
        if(Modules[i].icon.isVisible)
        {
            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _AGPipeline);

            Modules[i].icon.AGPC.movement.y = CBT+(float)(i*(CBTStatus*ratio)/40);
            Modules[i].icon.AGPC.movement.x = -0.98+(((CBTStatus-1)/20)/ratio);
            Modules[i].icon.AGPC.transformation = {(CBTStatus/20)/ratio,0.0,0.0,0.0,0.0,(CBTStatus/40)*ratio,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0};

            VkDeviceSize offset = 0;
                        
            vkCmdBindVertexBuffers(cmd, 0, 1, &Modules[i].icon.AG._vertexBuffer, &offset);
            
            vkCmdBindIndexBuffer(cmd, Modules[i].icon.AG.indexBuffer, 0, VK_INDEX_TYPE_UINT16);

            vkCmdPushConstants(cmd, _AGPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &Modules[i].icon.AGPC);

            vkCmdDrawIndexed(cmd, Modules[i].icon.AG.sizeOfIndices, 1, 0, 0, 0);
        }
    }
    DEBUG("after drawing icons");

    for(int i = 0; i < sizeOfMenuStuff;i++) // make it less than 18 lines per draw
    { 
        if(Modules[i].icon.isVisible) {

            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _AGPipeline);

            VkDeviceSize offset = 0;
            vkCmdBindVertexBuffers(cmd, 0, 1, &_AGA[BLACKLINE_ID].AG._vertexBuffer, &offset);

            vkCmdBindIndexBuffer(cmd, _AGA[BLACKLINE_ID].AG.indexBuffer, 0, VK_INDEX_TYPE_UINT16);

            AGPushConstants BLC = _AGA[0].AGPC;

            BLC.movement.y = CBT+(float)(i*(CBTStatus*ratio)/40);
            BLC.movement.x = -0.98+(((CBTStatus-1)/20)/ratio);
            BLC.transformation = {(CBTStatus/20)/ratio,0.0,0.0,0.0,0.0,CBTStatus/250,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0};
            
            vkCmdPushConstants(cmd, _AGPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &BLC);

            vkCmdDrawIndexed(cmd, _AGA[BLACKLINE_ID].AG.sizeOfIndices, 1, 0, 0, 0);
        }
    }
}

FrameData& get_current_frame()
{
	return _frames[_frameNumber % FRAME_OVERLAP];
}

// file handling
//
//
//
//
//

void init_fileConverter()
{
    FILE*MKTFILE = fopen("./files/files.MKTI","rb");
    if( MKTFILE != NULL )
    {
        char *charArray;
        fseek(MKTFILE, 0L, SEEK_END);
        long sizeOfFile = ftell(MKTFILE);
        rewind(MKTFILE);
        charArray = (char*)malloc(sizeOfFile);
        sizeOfFile = fread( charArray,1, sizeOfFile, MKTFILE );
        fclose( MKTFILE );
        _MKT_fileTypesSupported = 0;
        for(int i = 0; i <sizeOfFile;i++)_MKT_fileTypesSupported+=charArray[i]=='\n'?1:0;
        _MKT_fileTypes = (MKTfileImporter*)malloc(sizeof(MKTfileImporter)*_MKT_fileTypesSupported);
        int currentModule = 0;
        for(int i = 0; i < sizeOfFile;i++)
        {
            if(i==0||charArray[i-1]=='\n')
            {
                int offset;
                _MKT_fileTypes[currentModule].string = (char*)malloc(1);
                for(offset = 0; charArray[i+offset]!=' ';offset++)
                {   
                    _MKT_fileTypes[currentModule].string = (char*)realloc(_MKT_fileTypes[currentModule].string,offset+1);
                    _MKT_fileTypes[currentModule].string[offset] = charArray[i+offset];
                    _MKT_fileTypes[currentModule].sizeOfString = offset+1;
                }
                _MKT_fileTypes[currentModule].sizeOfString = offset+1;
                _MKT_fileTypes[currentModule].string = (char*)realloc(_MKT_fileTypes[currentModule].string,offset+1);
                _MKT_fileTypes[currentModule].string[offset] = 0;
                offset++;
                _MKT_fileTypes[currentModule].type = charArray[i+offset];

                char FFP[100]; // i give up

                snprintf(FFP,100,"./files/%s.%s",_MKT_fileTypes[currentModule].string,
                #ifdef _WIN32
                "dll"
                #elif __gnu_linux__
                "so"
                #endif
                );
                _MKT_fileTypes[currentModule].loadFile = 
                #ifdef _WIN32
                (void *(*)(char *FP,int*error))GetProcAddress(LoadLibrary(FFP), "_MKTP_read")
                #elif __gnu_linux__
                    dlopen(FFP,RTLD_NOW);
                #endif
                ;
            }
        }

    } else {
        printf("\nCannot find file file");
    }
}

int loadFile(char * filePath, char mode)
{
    int error = 0;
    if(mode == 0) // arrayGraphics
        0;
    else if(mode == 1) // images
    {
        MKTPic data  = _MKT_loadImage(filePath,&error);
        if(error == 0)
            return loading_MKTP_image(data);
        else
            printf("\nERROR DURING FILELOADING: %d",error);
    }
    else if(mode == 2) // objects
        0;
    else if(mode == 3) // data
        0;
    else if(mode == 4) // text
        0;
    return -1;
}

MKTPic _MKT_loadImage(char * filePath,int * error)
{
    int stringLenght;
    MKTPic returnPic;
    for(stringLenght = 0; filePath[stringLenght] != 0;stringLenght++)0;//printf("\ntwo things: %d %c",stringLenght, filePath[stringLenght]);
    int dotPlace;
    for(dotPlace = 0; filePath[stringLenght-dotPlace] != '.';dotPlace++)0;//printf("\ntwo things: %d %d %c",dotPlace,stringLenght, filePath[stringLenght-dotPlace]);
    FILE*MKTFILE = fopen("./files/files.MKTI","rb");
    if( MKTFILE != NULL )
    {
        char *charArray;
        fseek(MKTFILE, 0L, SEEK_END);
        long sizeOfFile = ftell(MKTFILE);
        rewind(MKTFILE);
        charArray = (char*)malloc(sizeOfFile);
        sizeOfFile = fread( charArray,1, sizeOfFile, MKTFILE );
        fclose( MKTFILE );
        int index = 0;
        
        char foundMatch = false;
        for(int i = 0; i < sizeOfFile&&!foundMatch;i++)
            if(i==0||charArray[i-1] == '\n') { index++;
                for(int offset = 0; offset <= dotPlace&&!foundMatch;offset++)
                    if(dotPlace!=offset+1&&charArray[i+offset]!=filePath[stringLenght-dotPlace+offset+1])
                        break;
                    else
                        if(dotPlace==offset+1&&charArray[i+offset]==' ')
                            foundMatch = true;}

        if(!foundMatch)
        {
            *error = 1;
            return returnPic;
        }
        free(charArray);
        index--;
        if(_MKT_fileTypes[index].type != 1)
        {
            *error = 2;
            return returnPic;
        }
        return *(MKTPic*)_MKT_fileTypes[index].loadFile(filePath,error);


    } else {
        printf("\nCannot find file file");
    }
    return returnPic;
}