#define _MKT_APP_ENGINE_THIRD_
#include <MKTAppEngine.h>
// MAIN FUNCIONS

void init()
{
    DEBUG("III init III");

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

    _VE_INIT_Sampler(); // #0000ff

    // _VE_INIT_OPENCL(); // #0000ff "Couldn't identify a platform"

    _VE_INIT_VE(); // #0000ff

    DEBUG("III> init <III");
    return;
}

void run()
{
    DEBUG("III run III");
    // glfwSetKeyCallback(_window, key_callback);

    while (!glfwWindowShouldClose(_window)) {
        _VE_RUN_drawFrame();
        RDEBUG("test123467");
        glfwPollEvents(); // #ff0000
        RDEBUG("test1234678");
    }

    vkDeviceWaitIdle(_device);

    DEBUG("III> run <III");
    return;
}

void cleanup()
{
    DEBUG("III cleanup III");

    DEBUG("II inside cleanup II");


    // MKTreturnDelQueue();  temporary disabled
// future MAKiT here, it's permanent
//(Unless I'll make a video about it or get a very good deal)

    MKTreturnError("./errors.log");

    DEBUG("II VK hard cleanup II");

    vkDestroySampler(_device, _textureSampler, NULL);

    DEBUG("II VK QDR cleanup II");

    for(IntDex i = 0; i < _ren_sizeOfQDR; i++)
    {
        for(int a = 0; a < _ren_QDR[i].sizeOfUniformBuffers; a++)
        {
            vkDestroyBuffer(_device,  _ren_QDR[i].uniformBuffers[a], NULL);
            vkFreeMemory(_device,  _ren_QDR[i].uniformBuffersMemory[a], NULL);
        }

        vkDestroyDescriptorPool(_device, _ren_QDR[i].descriptorPool, NULL);
        vkDestroyBuffer(_device, _ren_QDR[i].vertexBuffer, NULL);
        vkDestroyBuffer(_device, _ren_QDR[i].indexBuffer, NULL);
        vkFreeMemory(_device, _ren_QDR[i].vertexBufferMemory, NULL);
        vkFreeMemory(_device, _ren_QDR[i].indexBufferMemory, NULL);
        free(_ren_QDR[i]._dataQDR.vertices);
        free(_ren_QDR[i]._dataQDR.indices);
    }

    DEBUG("II VK PiC cleanup II");

    for(unsigned int i = 0; i < _ren_sizeOfPiC; i++)
    {
        for(int a = 0; a < _ren_PiC[i].sizeOfUniformBuffers; a++)
        {
            vkDestroyBuffer(_device,  _ren_PiC[i].uniformBuffers[a], NULL);
            vkFreeMemory(_device,  _ren_PiC[i].uniformBuffersMemory[a], NULL);
        }

        vkDestroyDescriptorPool(_device, _ren_PiC[i].descriptorPool, NULL);
        vkDestroyBuffer(_device, _ren_PiC[i].vertexBuffer, NULL);
        vkDestroyBuffer(_device, _ren_PiC[i].indexBuffer, NULL);
        vkFreeMemory(_device, _ren_PiC[i].vertexBufferMemory, NULL);
        vkFreeMemory(_device, _ren_PiC[i].indexBufferMemory, NULL);
        free(_ren_PiC[i]._dataPiC.vertices);
        free(_ren_PiC[i]._dataPiC.indices);


        vkDestroyImageView(_device, _ren_PiC[i].textureImageView, NULL);
        vkDestroyImage(_device, _ren_PiC[i].textureImage, NULL);
        vkFreeMemory(_device, _ren_PiC[i].textureImageMemory, NULL);
    }

    DEBUG("II VK BM cleanup II");

    for(unsigned int i = 0; i < _ren_sizeOfBM; i++)
    {
        for(int a = 0; a < _ren_BM[i].sizeOfUniformBuffers; a++)
        {
            vkDestroyBuffer(_device,  _ren_BM[i].uniformBuffers[a], NULL);
            vkFreeMemory(_device,  _ren_BM[i].uniformBuffersMemory[a], NULL);
        }

        vkDestroyDescriptorPool(_device, _ren_BM[i].descriptorPool, NULL);
        vkDestroyBuffer(_device, _ren_BM[i].vertexBuffer, NULL);
        vkDestroyBuffer(_device, _ren_BM[i].indexBuffer, NULL);
        vkFreeMemory(_device, _ren_BM[i].vertexBufferMemory, NULL);
        vkFreeMemory(_device, _ren_BM[i].indexBufferMemory, NULL);
        free(_ren_BM[i]._dataPiC.vertices);
        free(_ren_BM[i]._dataPiC.indices);


        vkDestroyImageView(_device, _ren_BM[i].textureImageView, NULL);
        vkDestroyImage(_device, _ren_BM[i].textureImage, NULL);
        vkFreeMemory(_device, _ren_BM[i].textureImageMemory, NULL);
    }

    DEBUG("II VK AG cleanup II");

    for(IntDex i = 0; i < _ren_sizeOfAG; i++)
    {
        for(int a = 0; a < _ren_AG[i].sizeOfUniformBuffers; a++)
        {
            vkDestroyBuffer(_device,  _ren_AG[i].uniformBuffers[a], NULL);
            vkFreeMemory(_device,  _ren_AG[i].uniformBuffersMemory[a], NULL);
        }

        vkDestroyDescriptorPool(_device, _ren_AG[i].descriptorPool, NULL);
        vkDestroyBuffer(_device, _ren_AG[i].vertexBuffer, NULL);
        vkDestroyBuffer(_device, _ren_AG[i].indexBuffer, NULL);
        vkFreeMemory(_device, _ren_AG[i].vertexBufferMemory, NULL);
        vkFreeMemory(_device, _ren_AG[i].indexBufferMemory, NULL);
        free(_ren_AG[i]._dataAG.vertices);
        free(_ren_AG[i]._dataAG.indices);
    }
    DEBUG("II VK materials cleanup II");
    for(int i = 0; i < _ren_sizeOfMaterials; i++)
    {
        vkDestroyDescriptorSetLayout(_device, _ren_materials[i].descriptorSetLayout, NULL);
        vkDestroyPipeline(_device,_ren_materials[i].graphicsPipeline,NULL);
        vkDestroyPipelineLayout(_device,_ren_materials[i].pipelineLayout,NULL);
    }

    DEBUG("II VK device cleanup II");
    vkDestroyCommandPool(_device, _commandPool, NULL);

    DEBUG("II VK runtimeKit cleanup II");
    for (size_t i = 0; i < _sizeOfRuntimeKit; i++) {
        vkDestroySemaphore(_device, _runtimeKit[i].imageAvailableSemaphore, NULL);
        vkDestroySemaphore(_device, _runtimeKit[i].renderFinishedSemaphore, NULL);
        vkDestroyFence(_device, _runtimeKit[i].inFlightFence, NULL);
    }

    DEBUG("II VK swapchainFrameBuffers cleanup II");
    for (IntDex i = 0; i < _sizeOfSwapChainFramebuffers;i++) {
        vkDestroyFramebuffer(_device, _swapChainFramebuffers[i], NULL);
    }

    DEBUG("II VK RenderPass cleanup II");
    vkDestroyRenderPass(_device, _renderPass, NULL);

    DEBUG("II VK SwapchainImageViews cleanup II");
    for (IntDex i = 0; i < _sizeOfSwapChainImageViews; i++) {
        vkDestroyImageView(_device, _swapChainImageViews[i], NULL);
    }

    DEBUG("II VK Swapchain cleanup II");
    vkDestroySwapchainKHR(_device, _swapChain, NULL);

    DEBUG("II VK device cleanup II");
    vkDestroyDevice(_device, NULL);

    DEBUG("II VK Surface cleanup II");
    vkDestroySurfaceKHR(_instance, _surface, NULL);

    DEBUG("II VK Instance cleanup II");
    vkDestroyInstance(_instance, NULL);

    DEBUG("II VK GLFWWindow cleanup II");
    glfwDestroyWindow(_window);

    DEBUG("II VK Glfw cleanup II");
    glfwTerminate();
    
    DEBUG("II VK Freeing cleanup II");

    free(_ren_materials);

    free(_swapChainImageViews);
    free(_swapChainImages);
    free(queueCreateInfos);
    free(_requiredExtensions.pString);
    free(validationLayers.pString);
    free(_extensions);

    DEBUG("III> cleanup <III");
    return;
}