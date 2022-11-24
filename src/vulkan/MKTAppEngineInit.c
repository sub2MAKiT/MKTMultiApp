#include <MKTAppEngine.h>
#include <pipeline/pipeline.h>
#include <fileLoading/fileLoading.h>

//====================================HELPER FUNCTIONS=====================================


char checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties * availableLayers;

    SAFEMALLOC(availableLayers, (sizeof(VkLayerProperties)*layerCount));

    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    for (IntDex i = 0; i < validationLayers.sizeOfString; i++) {
        char layerFound = 0;

        for (IntDex j = 0; j < layerCount; j++) {
            if (MKTCstrcmp(validationLayers.pString[i], availableLayers[j].layerName) == 0) {
                layerFound = 1;
                break;
            }
        }

        if (!layerFound) {
            free(availableLayers);
            return 0;
        }
    }

    free(availableLayers);
    return 1;
}

typedef struct TEMPQueueFamilyIndices {
    IntDex graphicsFamily;
    char real;

    IntDex presentFamily;
    char preal;
} QueueFamilyIndices;

typedef struct TEMPSwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;

    VkSurfaceFormatKHR * formats;
    IntDex sizeOfFormats;

    VkPresentModeKHR * presentModes;
    IntDex sizeOfPresentModes;

} SwapChainSupportDetails;

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, NULL);

    if (formatCount != 0) {
        SAFEMALLOC(details.formats, (sizeof(VkSurfaceFormatKHR)*formatCount));

        details.sizeOfFormats = formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats);
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, NULL);

    if (presentModeCount != 0) {
        SAFEMALLOC(details.presentModes, (sizeof(VkPresentModeKHR)*presentModeCount));

        details.sizeOfPresentModes = presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes);
    }

    return details;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;
    indices.real = 0;
    indices.preal = 0;
    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
    VkQueueFamilyProperties * queueFamilies;

    SAFEMALLOC(queueFamilies,(sizeof(VkQueueFamilyProperties)*queueFamilyCount));

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);



    int i = 0;
    for (int j = 0; j < queueFamilyCount; j++) {
        if (queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
            indices.real = 1;
        }

        VkBool32 presentSupport = 0;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, indices.graphicsFamily, _surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
            indices.preal = 1;
        }

        i++;
    }



    free(queueFamilies);

    return indices;
}

char checkDeviceExtensionsSupport(VkPhysicalDevice device)
{
    unsigned int extensionCount;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);

    VkExtensionProperties * availableExtensions;

    SAFEMALLOC(availableExtensions,sizeof(VkExtensionProperties)*extensionCount);

    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, availableExtensions);

    char found = 0;
    char foundn = 0;

    for(IntDex j = 0; j < _deviceExtensions.sizeOfString; j++)
    {
        for (IntDex i = 0; i < extensionCount && !found; i++) {
            if(weakMKTCstrcmp(_deviceExtensions.pString[j],availableExtensions[i].extensionName))
                found = 1;
            else
                if(i == extensionCount - 1)
                    foundn = 1;
        }
        found = 0;
    }

    free(availableExtensions);

    return !foundn;
}

VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR * capabilities) {
    if (capabilities->currentExtent.width != 4294967295) {
        return capabilities->currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);

        VkExtent2D actualExtent = {
            width,
            height
        };

        actualExtent.width = MKTClamp(actualExtent.width, capabilities->minImageExtent.width, capabilities->maxImageExtent.width);
        actualExtent.height = MKTClamp(actualExtent.height, capabilities->minImageExtent.height, capabilities->maxImageExtent.height);

        return actualExtent;
    }
}

VkPresentModeKHR chooseSwapPresentMode(VkPresentModeKHR * availablePresentModes, IntDex sizeOfAvailablePresentModes) {
    for (IntDex i = 0; i < sizeOfAvailablePresentModes; i++) {
        if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) { //VK_PRESENT_MODE_FIFO_RELAXED_KHR
            return availablePresentModes[i];
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(VkSurfaceFormatKHR * availableFormats, IntDex sizeOfAvailableFormats) {

    for (IntDex i = 0; i < sizeOfAvailableFormats; i++) {
        if (availableFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormats[i];
        }
    }

    return availableFormats[0];
}

MKTdeviceRating pickDeviceRating(VkPhysicalDevice device) {
    MKTdeviceRating DR;
    DR.rating = 0;
    DR.isSuitable = 0;

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    DR.rating += (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)*1000;
    DR.rating += deviceProperties.limits.maxImageDimension2D;

    QueueFamilyIndices indices = findQueueFamilies(device);

    SAFEMALLOC(queueCreateInfos, (sizeof(VkDeviceQueueCreateInfo)*2));
    sizeOfQueueCreateInfos = 2;

    unsigned int uniqueQueueFamilies[2] = {indices.graphicsFamily, indices.presentFamily};

    float queuePriority = 1.0f;
    for (int i = 0; i < 2; i++) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = uniqueQueueFamilies[i]; // problems on linux
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos[i] = queueCreateInfo;
    }

    char extensionsSupported = checkDeviceExtensionsSupport(device);

    char swapChainAdequate = 0;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = swapChainSupport.sizeOfFormats && swapChainSupport.sizeOfPresentModes;
    }

    DR.isSuitable = deviceFeatures.geometryShader && indices.real && indices.preal && extensionsSupported && swapChainAdequate;

    return DR;
}

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    // auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    _framebufferResized = 1;
}

//=========================================================================================

void _VE_INIT_window()
{
    // for creating a GLFW window
    DEBUG("II init:window II");

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Disable creation of an OpenGL context

    _window = glfwCreateWindow(_WindowS.width, _WindowS.height, "pls subscribe", NULL, NULL); // create window
    // glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);

    DEBUG("II> init:window <II");
    return;
}



void _VE_INIT_Instance()
{
    _deviceExtensions.sizeOfString = 1;

    SAFEMALLOC(_deviceExtensions.pString,sizeof(MKTString));

    _deviceExtensions.pString[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

    DEBUG("II init:instance II");
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        MKTerror(100);
    }

    validationLayers.sizeOfString = 1;
    SAFEMALLOC(validationLayers.pString,sizeof(MKTString));
    validationLayers.pString[0] = "VK_LAYER_KHRONOS_validation";

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = _MKT_APPNAME;
    appInfo.applicationVersion = VK_MAKE_VERSION(_MKT_MVER_Major, _MKT_MVER_Minor, _MKT_MVER_Patch);
    appInfo.pEngineName = _MKT_ENGNAME;
    appInfo.engineVersion = VK_MAKE_VERSION(_MKT_EVER_Major, _MKT_EVER_Minor, _MKT_EVER_Patch);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if (enableValidationLayers) {
    createInfo.enabledLayerCount = validationLayers.sizeOfString;
    createInfo.ppEnabledLayerNames = validationLayers.pString;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    SAFEMALLOC(_requiredExtensions.pString,sizeof(MKTString));

    for(uint32_t i = 0; i < glfwExtensionCount; i++) {
        _requiredExtensions.sizeOfString = i+1;
        _requiredExtensions.pString = realloc(_requiredExtensions.pString,(i+1)*sizeof(MKTString));
        _requiredExtensions.pString[i] = glfwExtensions[i];
    }

    createInfo.enabledExtensionCount = (uint32_t)_requiredExtensions.sizeOfString;
    createInfo.ppEnabledExtensionNames = _requiredExtensions.pString;

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);

    SAFEMALLOC(_extensions, (sizeof(VkExtensionProperties)*extensionCount));

    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, _extensions);

    VK_CHECK(vkCreateInstance(&createInfo, NULL, &_instance));

    DEBUG("II> init:instance <II");
    return;
}

void _VE_INIT_PhysicalDevice()
{
    DEBUG("II init:physical device II");
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance, &deviceCount, NULL);

    if (deviceCount == 0) {
        MKTerror(101);
    }

    VkPhysicalDevice * devices;

    SAFEMALLOC(devices,(sizeof(VkPhysicalDevice)*deviceCount));

    vkEnumeratePhysicalDevices(_instance, &deviceCount, devices);
    
    IntDex currentRecord = 0;
    for (IntDex i = 0; i < deviceCount; i++) {
        MKTdeviceRating tempRating = pickDeviceRating(devices[i]);
        if (tempRating.isSuitable && tempRating.rating > currentRecord) {
            _chosenGPU = devices[i];
            currentRecord = tempRating.rating; // my record: 33768
            break;
        }
    }

    if (_chosenGPU == VK_NULL_HANDLE) {
        MKTerror(101);
    }

    free(devices);

    DEBUG("II> init:physical device <II");
    return;
}

void _VE_INIT_LogicalDevice()
{
    DEBUG("II init:logical device II");
    QueueFamilyIndices indices = findQueueFamilies(_chosenGPU);

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = _deviceExtensions.sizeOfString;
    createInfo.ppEnabledExtensionNames =  _deviceExtensions.pString;


    createInfo.queueCreateInfoCount = sizeOfQueueCreateInfos;
    createInfo.pQueueCreateInfos = queueCreateInfos;

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = validationLayers.sizeOfString;
        createInfo.ppEnabledLayerNames = validationLayers.pString;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    VK_CHECK(vkCreateDevice(_chosenGPU, &createInfo, NULL, &_device));

    vkGetDeviceQueue(_device, indices.graphicsFamily, 0, &_graphicsQueue);
    vkGetDeviceQueue(_device, indices.presentFamily, 0, &_presentQueue);

    DEBUG("II> init:logical device <II");
    return;
}

void _VE_INIT_WindowSurface()
{
    VK_CHECK(glfwCreateWindowSurface(_instance, _window, NULL, &_surface));
    return;
}

void _VE_INIT_Swapchain()
{
    DEBUG("II init:swapchain II");
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(_chosenGPU);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats,swapChainSupport.sizeOfFormats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes,swapChainSupport.sizeOfPresentModes);
    VkExtent2D extent = chooseSwapExtent(&swapChainSupport.capabilities);

    _imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && _imageCount > swapChainSupport.capabilities.maxImageCount) {
        _imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _surface;
    createInfo.minImageCount = _imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(_chosenGPU);
    unsigned int queueFamilyIndices[2] = {indices.graphicsFamily, indices.presentFamily};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = NULL; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    // printf("fp: %llx\n",vkCreateSwapchainKHR);

    // printf("\n%x %x\n",(*(VkFramebufferCreateInfo*)&createInfo).renderPass,VK_NULL_HANDLE);

    DEBUG("marker 1");
    VK_CHECK(vkCreateSwapchainKHR(_device, &createInfo, NULL, &_swapChain));
    DEBUG("marker 2");

    vkGetSwapchainImagesKHR(_device, _swapChain, &_imageCount, NULL);
    SAFEMALLOC(_swapChainImages,(sizeof(VkImage)*_imageCount));
    vkGetSwapchainImagesKHR(_device, _swapChain, &_imageCount, _swapChainImages);
    _sizeOfSwapChainImages = _imageCount;

    
    _swapChainImageFormat = surfaceFormat.format;
    _swapChainExtent = extent;

    DEBUG("II> init:swapchain <II");
}

void _VE_INIT_ImageViews()
{
    DEBUG("II init:ImageViews II");
    SAFEMALLOC(_swapChainImageViews,sizeof(VkImageView)*_sizeOfSwapChainImages);
    _sizeOfSwapChainImageViews = _sizeOfSwapChainImages;

    for (size_t i = 0; i < _sizeOfSwapChainImages; i++) {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = _swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = _swapChainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VK_CHECK(vkCreateImageView(_device, &createInfo, NULL, &_swapChainImageViews[i]));
    }
    DEBUG("II> init:ImageViews <II");

}

void _VE_INIT_RenderPass()
{
    DEBUG("II init:RenderPass II");

    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = _swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VK_CHECK(vkCreateRenderPass(_device, &renderPassInfo, NULL, &_renderPass));

    DEBUG("II> init:RenderPass <II");
    return;
}

void _VE_INIT_Framebuffers()
{
    DEBUG("II init:Framebuffers II");

    _sizeOfSwapChainFramebuffers = _sizeOfSwapChainImageViews;
    SAFEMALLOC(_swapChainFramebuffers,(_sizeOfSwapChainImageViews*sizeof(VkFramebuffer)))

    for (size_t i = 0; i < _sizeOfSwapChainImageViews; i++) {

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = _renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &_swapChainImageViews[i];
        framebufferInfo.width = _swapChainExtent.width;
        framebufferInfo.height = _swapChainExtent.height;
        framebufferInfo.layers = 1;

        VK_CHECK(vkCreateFramebuffer(_device, &framebufferInfo, NULL, &_swapChainFramebuffers[i]));
    }
    
    DEBUG("II> init:Framebuffers <II");
    return;
}

void _VE_INIT_CommandPool()
{
    DEBUG("II init:CommandPool II");

    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(_chosenGPU);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

    VK_CHECK(vkCreateCommandPool(_device, &poolInfo, NULL, &_commandPool));

    DEBUG("II> init:CommandPool <II");
    return;
}

void _VE_INIT_CommandBuffer()
{
    DEBUG("II init:CommandBuffer II");

    _sizeOfCommandBuffers = MAX_FRAMES_IN_FLIGHT;

    SAFEMALLOC(_commandBuffers,(sizeof(VkCommandBuffer)*_sizeOfCommandBuffers));

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = _sizeOfCommandBuffers;

    VK_CHECK(vkAllocateCommandBuffers(_device, &allocInfo, _commandBuffers));

    DEBUG("II> init:CommandBuffer <II");
    return;
}

void _VE_INIT_SyncObjects()
{
    DEBUG("II init:SyncObjects II");

    _sizeOfRuntimeKit = MAX_FRAMES_IN_FLIGHT;
    SAFEMALLOC(_runtimeKit,(sizeof(MKTVKruntime)*_sizeOfRuntimeKit));

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < _sizeOfRuntimeKit; i++) {
        if (vkCreateSemaphore(_device, &semaphoreInfo, NULL, &_runtimeKit[i].imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(_device, &semaphoreInfo, NULL, &_runtimeKit[i].renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(_device, &fenceInfo, NULL, &_runtimeKit[i].inFlightFence) != VK_SUCCESS) {
        printf("error sync");
    }
  }

    DEBUG("II> init:SyncObjects <II");
    return;
}

void _VE_INIT_Sampler()
{
    DEBUG("II init:Sampler II");

    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1.0f;

    VK_CHECK(vkCreateSampler(_device, &samplerInfo, NULL, &_textureSampler));

    DEBUG("II> init:Sampler <II");
    return;
}

void _VE_INIT_GraphicPipelines()
{
    DEBUG("II init:GraphicPipelines II");

    SAFEMALLOC(_ren_materials,sizeof(MKTmaterial));
    _ren_sizeOfMaterials = 0;

    _MKT_LOAD_PIPELINE("shaders/AGShader.vert.spv","shaders/AGShader.frag.spv",0); // 0 = MKTAG
    _MKT_LOAD_PIPELINE("shaders/PiCShader.vert.spv","shaders/PiCShader.frag.spv",1); // 1 = MKTPiC
    _MKT_LOAD_PIPELINE("shaders/BMShader.vert.spv","shaders/BMShader.frag.spv",2); // 2 = MKTbm
    _MKT_LOAD_PIPELINE("shaders/qdrShader.vert.spv","shaders/qdrShader.frag.spv",2); // 2 = MKTbm

    // _MKT_LOAD_PIPELINE("shaders/AGShader.vert.spv","shaders/AGShader.frag.spv", 0,0);

    DEBUG("II> init:GraphicPipelines <II");
    return;
}

void _VE_INIT_VE()
{
    DEBUG("II init:VEvariables II");

    _ren_sizeOfAG = 0;

    SAFEMALLOC(_ren_AG,sizeof(MKTag));

    _ren_sizeOfBM = 0;

    SAFEMALLOC(_ren_BM,sizeof(MKTag));

    // _MKT_genBM(PiCVertex * inVertices,IntDex inSizeOfVertices,unsigned int * inIndices,IntDex inSizeOfIndices);

    loadFile("../utils/test.MKTbm");

    loadFile("../utils/test.MKTP");

    // loadFile("../utils/test.MKTP",MKTPICV);

    // loadFile("DataFiles/testlogo.MKTRAWPiC",MKTPICV);

    // loadFile("./DataFiles/shape.MKTAG",MKTAGV);

    printf("%f %f %f %f %.27f %f\n",_MKTAS_CALC_E(1),_MKTAS_CALC_E(2),_MKTAS_CALC_E(3),_MKTAS_CALC_E(5),_MKTAS_CALC_E(26),_MKTAS_CALC_EXP(10,2.0));

    // for(int i = 1; i <= 26; i++)
    // {
    //     printf("%d. %.27f\n",i,_MKTAS_CALC_E(i));
    // }

    DEBUG("II> init:VEvariables <II");
    return;
}