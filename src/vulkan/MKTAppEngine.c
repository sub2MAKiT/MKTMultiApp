#define MAIN_MKT_APP_ENGINE_INCLUDE
#include "MKTAppEngine.h"
#include <MKTGNUbasedHeaders/MKTmath.h>
#include <iostream>

#include <SDL/SDL.h>
#include<SDL/SDL_vulkan.h>

#include <fstream>

#include "vkb/VkBootstrap.h"

#define VK_CHECK(x)                                             \
do                                                              \
{                                                               \
    VkResult err = x;                                           \
    if (err)                                                    \
    {                                                           \
        std::cout << "Detected Vulkan error: " << err << std::endl;                                      \
        abort();                                                \
    }                                                           \
} while (0)

#ifdef MKT_DEBUG
int step = 0;
#define DEBUG(x)              \
printf_s("\033[%d;40m[%d]" x "\033[0;40m\n",int((rand() % 6) + 1 + MKTfloor(float(((rand() % 2) + 1)*1.5)) * 30),step);step++;
#else
#define DEBUG(x)                                                \
if (1 == 2)printf(x);
#endif

void _MKTGE_init() {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

    _window = SDL_CreateWindow(
        "Ventum engine", //#aaaa00
        SDL_WINDOWPOS_UNDEFINED, //#aaaa00
        SDL_WINDOWPOS_UNDEFINED, //#aaaa00
        _windowExtent.width, //#aaaa00
        _windowExtent.height, //#aaaa00
        window_flags //#aaaa00
    );

    _MKTGE_init_vulkan(); //#0000ff

    _MKTGE_init_swapchain(); //#0000ff

    _MKTGE_init_commands(); //#0000ff

    _MKTGE_init_default_renderpass(); //#0000ff

    _MKTGE_init_framebuffers(); //#0000ff

    _MKTGE_init_sync_structures(); //#0000ff

    _MKTGE_init_pipelines(); //#0000ff

    _isInitialized = true;
}

void _MKTGE_cleanup() {
    if (_isInitialized) {

        vkDestroyCommandPool(_device, _commandPool, NULL);

        vkDestroySwapchainKHR(_device, _swapchain, NULL);

        vkDestroyRenderPass(_device, _renderPass, NULL);

        for (int i = 0; i < _framebuffers.size(); i++)
        {
            vkDestroyFramebuffer(_device, _framebuffers[i], NULL);

            vkDestroyImageView(_device,_swapchainImageViews[i], NULL);
        }

        for (int i = 0; i < _swapchainImageViews.size(); i++) {

            vkDestroyImageView(_device, _swapchainImageViews[i], NULL);
        }

        vkDestroyDevice(_device,NULL);
        vkDestroySurfaceKHR(_instance, _surface, NULL);
        vkb::destroy_debug_utils_messenger(_instance, _debug_messenger);
        vkDestroyInstance(_instance, NULL);
        SDL_DestroyWindow(_window);
        DEBUG("III cleaned up III");
    }
}

void _MKTGE_draw() {
    //it don't be drawing
    VK_CHECK(vkWaitForFences(_device, 1, &_renderFence, true, 1000000000));
    VK_CHECK(vkResetFences(_device, 1, &_renderFence));

    uint32_t swapchainImageIndex;
    VK_CHECK(vkAcquireNextImageKHR(_device, _swapchain, 1000000000, _presentSemaphore, NULL, &swapchainImageIndex));

    VK_CHECK(vkResetCommandBuffer(_mainCommandBuffer, 0));

    VkCommandBuffer cmd = _mainCommandBuffer;

    VkCommandBufferBeginInfo cmdBeginInfo = {};
    cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBeginInfo.pNext = NULL;

    cmdBeginInfo.pInheritanceInfo = NULL;
    cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

    VkClearValue clearValue;
    clearValue.color = {{0.0f, 0.0f, 1.0f, 1.0f}};

    VkRenderPassBeginInfo rpInfo = {};
    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpInfo.pNext = NULL;

    rpInfo.renderPass = _renderPass;
    rpInfo.renderArea.offset.x = 0;
    rpInfo.renderArea.offset.y = 0;
    rpInfo.renderArea.extent = _windowExtent;
    rpInfo.framebuffer = _framebuffers[swapchainImageIndex];

    rpInfo.clearValueCount = 1;
    rpInfo.pClearValues = &clearValue;

    vkCmdBeginRenderPass(cmd, &rpInfo, VK_SUBPASS_CONTENTS_INLINE); //#00ff00

//#00ff00
//#00ff00

vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _trianglePipeline);

vkCmdDraw(cmd, 3, 1, 0, 0);

//#00ff00
//#00ff00

    vkCmdEndRenderPass(cmd);

    VK_CHECK(vkEndCommandBuffer(cmd));

    VkSubmitInfo submit = {};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.pNext = NULL;

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    submit.pWaitDstStageMask = &waitStage;

    submit.waitSemaphoreCount = 1;
    submit.pWaitSemaphores = &_presentSemaphore;

    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = &_renderSemaphore;

    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &cmd;

    VK_CHECK(vkQueueSubmit(_graphicsQueue, 1, &submit, _renderFence));

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.swapchainCount = 1;
    presentInfo.pWaitSemaphores = &_renderSemaphore;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pSwapchains = &_swapchain;

    presentInfo.pImageIndices = &swapchainImageIndex;

    VK_CHECK(vkQueuePresentKHR(_graphicsQueue, &presentInfo));

    _frameNumber++;
}

void _MKTGE_run() {
    SDL_Event e;
    bool bQuit = false;

    DEBUG("III started running III");

    while (!bQuit)
    {
         _MKTGE_draw();
        while (SDL_PollEvent(&e) != 0)
        {
                if (e.type == SDL_QUIT) bQuit = true;
        }
    }
    DEBUG("III ran III");
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

void _MKTGE_init_vulkan() //#0000ff
{
    vkb::InstanceBuilder builder;

    auto inst_ret = builder.set_app_name("Ventum engine")
        .request_validation_layers(true)//#aaaa00
        .require_api_version(1, 3, 0)//#aaaa00
        .use_default_debug_messenger()
		.build();

    vkb::Instance vkb_inst = inst_ret.value();

    _instance = vkb_inst.instance;

    _debug_messenger = vkb_inst.debug_messenger;

    SDL_Vulkan_CreateSurface(_window, _instance, &_surface);

    vkb::PhysicalDeviceSelector selector{ vkb_inst };
	vkb::PhysicalDevice physicalDevice = selector
		.set_minimum_version(1, 1)
		.set_surface(_surface)
		.select()
		.value();

        vkb::DeviceBuilder deviceBuilder{ physicalDevice };

	    vkb::Device vkbDevice = deviceBuilder.build().value();

        _device = vkbDevice.device;
	    _chosenGPU = physicalDevice.physical_device;

        _graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
        _graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

    DEBUG("II init:vulkan II");
}

void _MKTGE_init_swapchain() //#0000ff
{
    vkb::SwapchainBuilder swapchainBuilder{_chosenGPU,_device,_surface };

    vkb::Swapchain vkbSwapchain = swapchainBuilder
		.use_default_format_selection()
        .set_desired_present_mode(VK_PRESENT_MODE_MAILBOX_KHR)
        .set_desired_extent(_windowExtent.width, _windowExtent.height)
		.build()
		.value();

        _swapchain = vkbSwapchain.swapchain;
	    _swapchainImages = vkbSwapchain.get_images().value();
	    _swapchainImageViews = vkbSwapchain.get_image_views().value();

        _swapchainImageFormat = vkbSwapchain.image_format;

    DEBUG("II init:swapchain II");
}

void _MKTGE_init_commands()
{
    VkCommandPoolCreateInfo commandPoolInfo = command_pool_create_info(_graphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    VK_CHECK(vkCreateCommandPool(_device, &commandPoolInfo, NULL, &_commandPool));

    VkCommandBufferAllocateInfo cmdAllocInfo = command_buffer_allocate_info(_commandPool,1,VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    VK_CHECK(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &_mainCommandBuffer));

    DEBUG("II init:commands II");
}

void _MKTGE_init_default_renderpass()
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
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    VkRenderPassCreateInfo render_pass_info = {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 1;
	render_pass_info.pAttachments = &color_attachment;
    render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;

    VK_CHECK(vkCreateRenderPass(_device, &render_pass_info, NULL, &_renderPass));

    DEBUG("II init:renderPass II");
}

void _MKTGE_init_framebuffers()
{
    VkFramebufferCreateInfo fb_info = {};
	fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fb_info.pNext = NULL;

	fb_info.renderPass = _renderPass;
	fb_info.attachmentCount = 1;
	fb_info.width = _windowExtent.width;
	fb_info.height = _windowExtent.height;
	fb_info.layers = 1;

    const uint32_t swapchain_imagecount = _swapchainImages.size();
	_framebuffers = std::vector<VkFramebuffer>(swapchain_imagecount);

    for (int i = 0; i < swapchain_imagecount; i++) {

		fb_info.pAttachments = &_swapchainImageViews[i];
        if((&fb_info)->renderPass != VK_NULL_HANDLE)
        {
		    VK_CHECK(vkCreateFramebuffer(_device, &fb_info, NULL, &_framebuffers[i]));
        }
	}
}

void _MKTGE_init_sync_structures()
{
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = NULL;

    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VK_CHECK(vkCreateFence(_device, &fenceCreateInfo, NULL, &_renderFence));

    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.flags = 0;

    VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, NULL, &_presentSemaphore));
    VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, NULL, &_renderSemaphore));

    DEBUG("II init:sync structures II");
}

bool _MKTGE_load_shader_module(const char* filePath, VkShaderModule* outShaderModule)
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
    createInfo.pNext = NULL;

    createInfo.codeSize = buffer.size() * sizeof(uint32_t);
    createInfo.pCode = buffer.data();

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(_device, &createInfo, NULL, &shaderModule) != VK_SUCCESS) {
        return false;
    }
    *outShaderModule = shaderModule;
    DEBUG("II load:shaders II");
    return true;
}

void _MKTGE_init_pipelines() {
    VkShaderModule triangleFragShader;
    if (!_MKTGE_load_shader_module("./build/shaders/triangle.frag.spv", &triangleFragShader))
    {
        DEBUG("triangle frag error");
    }
    else {
        DEBUG("triangle frag loaded");
    }

    VkShaderModule triangleVertexShader;
    if (!_MKTGE_load_shader_module("./build/shaders/triangle.vert.spv", &triangleVertexShader))
    {
        DEBUG("triangle vert error");
    }
    else {
        DEBUG("triangle vert loaded");
    }

    VkPipelineLayoutCreateInfo pipeline_layout_info = pipeline_layout_create_info();

    VK_CHECK(vkCreatePipelineLayout(_device, &pipeline_layout_info, NULL, &_trianglePipelineLayout));
    
    _shaderStages.push_back(
        pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, triangleVertexShader)
    );

    _shaderStages.push_back(
        pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, triangleFragShader)
    );

    _vertexInputInfo = vertex_input_state_create_info();

    _inputAssembly = input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    _viewport.x = 0.0f;
    _viewport.y = 0.0f;
    _viewport.width = (float)_windowExtent.width;
    _viewport.height = (float)_windowExtent.height;
    _viewport.minDepth = 0.0f;
    _viewport.maxDepth = 1.0f;

    _scissor.offset = {0, 0};
    _scissor.extent = _windowExtent;

    _rasterizer = rasterization_state_create_info(VK_POLYGON_MODE_FILL);

    _multisampling = multisampling_state_create_info();

    _colorBlendAttachment = color_blend_attachment_state();

    _pipelineLayout = _trianglePipelineLayout;

    _trianglePipeline = build_pipeline(_device, _renderPass);
}
//----------------------------------------------------------------------------------------------------------------------------------------

VkPipeline build_pipeline(VkDevice device, VkRenderPass pass)
{
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = NULL;

    viewportState.viewportCount = 1;
    viewportState.pViewports = &_viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &_scissor;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.pNext = NULL;

    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &_colorBlendAttachment;

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = NULL;

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

    VkPipeline newPipeline;
    if (vkCreateGraphicsPipelines(
        device, VK_NULL_HANDLE, 1, &pipelineInfo , NULL, &newPipeline) != VK_SUCCESS) {
            std::cout << "failed to create pipeline\n";
            return VK_NULL_HANDLE;
        }
        else {
            return newPipeline;
        }
}

// inits

VkCommandPoolCreateInfo command_pool_create_info(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags)
{
    VkCommandPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.pNext = NULL;

    info.queueFamilyIndex = queueFamilyIndex;
    info.flags = flags;
    return info;
}

VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool pool, uint32_t count, VkCommandBufferLevel level)
{
    VkCommandBufferAllocateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = NULL;

    info.commandPool = pool;
    info.commandBufferCount = count;
    info.level = level;
    return info;
}

VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule shaderModule)
{
    VkPipelineShaderStageCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.pNext = NULL;

    info.stage = stage;
    info.module = shaderModule;
    info.pName = "main";
    return info;
}

VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info() {
    VkPipelineVertexInputStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    info.pNext = NULL;

    info.vertexBindingDescriptionCount = 0;
    info.vertexAttributeDescriptionCount = 0;
    return info;
}

VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info(VkPrimitiveTopology topology) {
    VkPipelineInputAssemblyStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    info.pNext = NULL;

    info.topology = topology;
    info.primitiveRestartEnable = VK_FALSE;
    return info;
}

VkPipelineRasterizationStateCreateInfo rasterization_state_create_info(VkPolygonMode polygonMode)
{
    VkPipelineRasterizationStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.pNext = NULL;

    info.depthClampEnable = VK_FALSE;
    info.rasterizerDiscardEnable = VK_FALSE;

    info.polygonMode = polygonMode;
    info.lineWidth = 1.0f;
    info.cullMode = VK_CULL_MODE_NONE;
    info.frontFace = VK_FRONT_FACE_CLOCKWISE;
    info.depthBiasEnable = VK_FALSE;
    info.depthBiasConstantFactor = 0.0f;
    info.depthBiasClamp = 0.0f;
    info.depthBiasSlopeFactor = 0.0f;

    return info;
}

VkPipelineMultisampleStateCreateInfo multisampling_state_create_info()
{
    VkPipelineMultisampleStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info.pNext = NULL;

    info.sampleShadingEnable = VK_FALSE;
    info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    info.minSampleShading = 1.0f;
    info.pSampleMask = NULL;
    info.alphaToCoverageEnable = VK_FALSE;
    info.alphaToOneEnable = VK_FALSE;
    return info;
}

VkPipelineColorBlendAttachmentState color_blend_attachment_state() {
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    return colorBlendAttachment;
}

VkPipelineLayoutCreateInfo pipeline_layout_create_info()
    {
        VkPipelineLayoutCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        info.pNext = NULL;

        info.flags = 0;
        info.setLayoutCount = 0;
        info.pSetLayouts = NULL;
        info.pushConstantRangeCount = 0;
        info.pPushConstantRanges = NULL;
        return info;
    }
