#include <MKTAppEngine.h>

void _VE_UTILS_CleanupSwapChain()
{
    for (size_t i = 0; i < _sizeOfSwapChainFramebuffers; i++) {
        vkDestroyFramebuffer(_device, _swapChainFramebuffers[i], NULL);
    }

    for (size_t i = 0; i < _sizeOfSwapChainImageViews; i++) {
        vkDestroyImageView(_device, _swapChainImageViews[i], NULL);
    }

    vkDestroySwapchainKHR(_device, _swapChain, NULL);
}

void _VE_UTILS_RecreateSwapChain()
{
    DEBUG("II utils:RecreateSwapChain II");

    int width = 0, height = 0;
    glfwGetFramebufferSize(_window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(_window, &width, &height);
        glfwWaitEvents();
    }
    
    vkDeviceWaitIdle(_device);

    _VE_UTILS_CleanupSwapChain();


    _VE_INIT_Swapchain();
    _VE_INIT_ImageViews();
    _VE_INIT_Framebuffers();
    DEBUG("II> utils:RecreateSwapChain <II");
    return;
}