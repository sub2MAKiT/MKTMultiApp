#define _MKT_APP_ENGINE_THIRD_
#include <MKTAppEngine.h>
// MAIN FUNCIONS

void init()
{

    _VE_INIT_window(); // #0000ff

    _VE_INIT_Instance(); // #0000ff

    _VE_INIT_WindowSurface(); // #0000ff

    _VE_INIT_PhysicalDevice(); // #0000ff

    _VE_INIT_LogicalDevice(); // #0000ff

    _VE_INIT_Swapchain(); // #0000ff

    _VE_INIT_ImageViews(); // #0000ff

    _VE_INIT_RenderPass(); // #0000ff

    _VE_INIT_GraphicPipelines(); // #0000ff

    _VE_INIT_Framebuffers(); // #0000ff

    _VE_INIT_CommandPool(); // #0000ff

    _VE_INIT_CommandBuffer(); // #0000ff

    _VE_INIT_SyncObjects(); // #0000ff

    DEBUG("III init III");
    DEBUG("III> init <III");
    return;
}

void run()
{
    DEBUG("III run III");

    while (!glfwWindowShouldClose(_window)) {
        _VE_RUN_drawFrame();
        glfwPollEvents();
    }

    vkDeviceWaitIdle(_device);

    DEBUG("III> run <III");
    return;
}

void cleanup()
{
    DEBUG("III cleanup III");

    DEBUG("II inside cleanup II");
    MKTreturnDelQueue();
    MKTreturnError("./errors.log");

    DEBUG("II VK hard cleanup II");

    vkDestroyCommandPool(_device, _commandPool, NULL);

    for (size_t i = 0; i < _sizeOfRuntimeKit; i++) {
        vkDestroySemaphore(_device, _runtimeKit[i].imageAvailableSemaphore, NULL);
        vkDestroySemaphore(_device, _runtimeKit[i].renderFinishedSemaphore, NULL);
        vkDestroyFence(_device, _runtimeKit[i].inFlightFence, NULL);
    }
    for (IntDex i = 0; i < _sizeOfSwapChainFramebuffers;i++) {
        vkDestroyFramebuffer(_device, _swapChainFramebuffers[i], NULL);
    }

    vkDestroyRenderPass(_device, _renderPass, NULL);

    for (IntDex i = 0; i < _sizeOfSwapChainImageViews; i++) {
        vkDestroyImageView(_device, _swapChainImageViews[i], NULL);
    }

    vkDestroySwapchainKHR(_device, _swapChain, NULL);

    vkDestroyDevice(_device, NULL);

    vkDestroySurfaceKHR(_instance, _surface, NULL);

    vkDestroyInstance(_instance, NULL);

    glfwDestroyWindow(_window);

    glfwTerminate();

    free(_REN_materials);

    free(_swapChainImageViews);
    free(_swapChainImages);
    free(queueCreateInfos);
    free(_requiredExtensions.pString);
    free(validationLayers.pString);
    free(_extensions);

    DEBUG("III> cleanup <III");
    return;
}