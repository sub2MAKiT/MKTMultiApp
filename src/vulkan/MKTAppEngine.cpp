#include "MKTAppEngine.h"
#include "../fileManagment/MKTarrayGraphics.h"
#define VMA_IMPLEMENTATION
#include <VulkanMA/vk_mem_alloc.h>
#include "../DEBUG.h"
#include "imageHandling.h"

// Some basig graphics:

#define BLACKLINE_ID 0
#define MENUARRAYGRAPHIC 4
#define MENULINEHORIZONTAL 5

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

void VentumEngine::init() {

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

    init_vulkan(); //#0000ff

    init_swapchain(); //#0000ff

    init_commands(); //#0000ff

    int why = loading_MKTP_image(MKTPicReader("./graphics/MKTPhotos/logo.MKTP")); // #ff0000

    init_createTextureSampler(); // #0000ff

    init_default_renderpass(); //#0000ff

    init_framebuffers(); //#0000ff

    init_sync_structures(); //#0000ff

    init_descriptors(); //#0000ff

    init_pipelines(); //#0000ff

    load_meshes(); //#0000ff

    load_AG(); //#0000ff

    loadMenuAG();

    init_scene(); //#0000ff

    _isInitialized = true;
}

void VentumEngine::cleanup() {
    if (_isInitialized) {

        vkWaitForFences(_device, 1, &get_current_frame()._renderFence, true, 1000000000);

        _mainDeletionQueue.flush();

        vkDestroySurfaceKHR(_instance, _surface, nullptr);

        vkDestroyDevice(_device, nullptr);
        vkDestroyInstance(_instance, nullptr);

        SDL_DestroyWindow(_window);

        DEBUG("III cleaned up III");
    }
}

void VentumEngine::draw() {
    //it don't be drawing
    VK_CHECK(vkWaitForFences(_device, 1, &get_current_frame()._renderFence, true, 1000000000));
    VK_CHECK(vkResetFences(_device, 1, &get_current_frame()._renderFence));

    uint32_t swapchainImageIndex;
    VK_CHECK(vkAcquireNextImageKHR(_device, _swapchain, 1000000000, get_current_frame()._presentSemaphore, nullptr, &swapchainImageIndex));

    VK_CHECK(vkResetCommandBuffer(get_current_frame()._mainCommandBuffer, 0));

    VkCommandBuffer cmd = get_current_frame()._mainCommandBuffer;

    VkCommandBufferBeginInfo cmdBeginInfo = {};
    cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBeginInfo.pNext = nullptr;

    cmdBeginInfo.pInheritanceInfo = nullptr;
    cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

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
    rpInfo.renderArea.extent = _windowExtent;
    rpInfo.framebuffer = _framebuffers[swapchainImageIndex];

    rpInfo.clearValueCount = 2;

    VkClearValue clearValues[] = {clearValue, depthClear};

    rpInfo.pClearValues = &clearValues[0];

    vkCmdBeginRenderPass(cmd, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);

//#00ff00
//#00ff00
    if(_selectedShader%2==1)
	    draw_objects(cmd, _renderables.data(), _renderables.size());
    else
        draw_AG(cmd,  _AGA.data(),  _AGA.size());

    drawMenu(cmd,Modules, sizeOfModules);
//#00ff00
//#00ff00

    vkCmdEndRenderPass(cmd);

    VK_CHECK(vkEndCommandBuffer(cmd));

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

    VK_CHECK(vkQueueSubmit(_graphicsQueue, 1, &submit, get_current_frame()._renderFence));

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.swapchainCount = 1;
    presentInfo.pWaitSemaphores = &get_current_frame()._renderSemaphore;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pSwapchains = &_swapchain;

    presentInfo.pImageIndices = &swapchainImageIndex;

    VK_CHECK(vkQueuePresentKHR(_graphicsQueue, &presentInfo));

    _frameNumber++;
}

void VentumEngine::run() {
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
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) bQuit = true;
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_RIGHT)
                    xSize += 0.01;
                else if(e.key.keysym.sym == SDLK_LEFT)
                    xSize -= 0.01;
                else if(e.key.keysym.sym == SDLK_UP)
                    ySize += 0.01;
                else if(e.key.keysym.sym == SDLK_DOWN)
                    ySize -= 0.01;
                else if(e.key.keysym.sym == SDLK_s)
                    yMove += 0.1;
                else if(e.key.keysym.sym == SDLK_w)
                    yMove -= 0.1;
                else if(e.key.keysym.sym == SDLK_d)
                    xMove += 0.1;
                else if(e.key.keysym.sym == SDLK_a)
                    xMove -= 0.1;
                else if(e.key.keysym.sym == SDLK_SPACE)
                    _selectedShader++;
                else if(e.key.keysym.sym == SDLK_q)
                    CBT++;
                else if(e.key.keysym.sym == SDLK_e)
                    isVisible -= (isVisible*2-1);
            }
            else if(e.type == SDL_MOUSEWHEEL)
                 if(e.wheel.y > 0)
                {
                     CBT += 0.01;
                }
                else if(e.wheel.y < 0)
                {
                     CBT -= 0.01;
                }
            else if(e.type == SDL_MOUSEBUTTONDOWN)
                if(e.button.button == SDL_BUTTON_LEFT)
                    printf("is it a float %f or an int %d",e.button.x,e.button.x);
                

            if(currentObjectToDraw > 2)
                currentObjectToDraw = 0;

            // for(int i = 0; i < _AGA.size();i++)
            // {
                // printf("\nobject number %d, is visible %d, movement %f %f %f, size %f %f %f %f",_AGA[i].isVisible,_AGA[i].AGPC.movement[0],_AGA[i].AGPC.movement[1],_AGA[i].AGPC.movement[2],_AGA[i].AGPC.transformation[0],_AGA[i].AGPC.transformation.value,_AGA[i].AGPC.transformation[10],_AGA[i].AGPC.transformation[15]);
            // }
        }
    }
    DEBUG("III ran III");
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

void VentumEngine::init_vulkan() //#0000ff
{
    vkb::InstanceBuilder builder;

    auto inst_ret = builder.set_app_name("MKTMultiApp")
        .request_validation_layers(true)//#aaaa00
        .require_api_version(1, 1, 0)//#aaaa00
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

        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.physicalDevice = _chosenGPU;
        allocatorInfo.device = _device;
        allocatorInfo.instance = _instance;
        vmaCreateAllocator(&allocatorInfo, &_allocator);

    DEBUG("II init:vulkan II");
}

void VentumEngine::init_swapchain() //#0000ff
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

    DEBUG("II init:swapchain II");
}

void VentumEngine::init_commands()
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

void VentumEngine::init_default_renderpass()
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

void VentumEngine::init_framebuffers()
{
    VkFramebufferCreateInfo fb_info = {};
	fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fb_info.pNext = nullptr;

	fb_info.renderPass = _renderPass; // this failes, time to fix it #ff0000
	fb_info.attachmentCount = 1;
	fb_info.width = _windowExtent.width;
	fb_info.height = _windowExtent.height;
	fb_info.layers = 1;

    const uint32_t swapchain_imagecount = _swapchainImages.size();
	_framebuffers = std::vector<VkFramebuffer>(swapchain_imagecount);

    for (int i = 0; i < swapchain_imagecount; i++) 
    {
        VkImageView attachments[2];
		attachments[0] = _swapchainImageViews[i];
	    attachments[1] = _depthImageView;

        
        fb_info.pAttachments = attachments;
        fb_info.attachmentCount = 2;

        VK_CHECK(vkCreateFramebuffer(_device, &fb_info, nullptr, &_framebuffers[i]));

        _mainDeletionQueue.push_function([=]() {
            vkDestroyFramebuffer(_device, _framebuffers[i], nullptr);
            vkDestroyImageView(_device, _swapchainImageViews[i], nullptr);
        });
	}
}

void VentumEngine::init_sync_structures()
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

bool VentumEngine::load_shader_module(const char* filePath, VkShaderModule* outShaderModule)
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

void VentumEngine::init_pipelines() {
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

    VkPipelineLayoutCreateInfo AG_pipeline_layout_info = vkinit::pipeline_layout_create_info();

    VkPushConstantRange AGpush_constant;
	AGpush_constant.offset = 0;
	AGpush_constant.size = sizeof(AGPushConstants);
	AGpush_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	AG_pipeline_layout_info.pPushConstantRanges = &AGpush_constant;
	AG_pipeline_layout_info.pushConstantRangeCount = 1;

    VK_CHECK(vkCreatePipelineLayout(_device, &AG_pipeline_layout_info, nullptr, &_AGPipelineLayout));

    VertexInputDescription AGvertexDescription = Vertex::getAG_vertex_description();

	pipelineBuilder._vertexInputInfo.pVertexAttributeDescriptions = AGvertexDescription.attributes.data();
	pipelineBuilder._vertexInputInfo.vertexAttributeDescriptionCount = AGvertexDescription.attributes.size();

	pipelineBuilder._vertexInputInfo.pVertexBindingDescriptions = AGvertexDescription.bindings.data();
	pipelineBuilder._vertexInputInfo.vertexBindingDescriptionCount = AGvertexDescription.bindings.size();

	pipelineBuilder._shaderStages.clear();

    VkShaderModule AGFragShader;
    if (!load_shader_module("./shaders/DiS.frag.spv", &AGFragShader))
    {
        std::cout << std::endl << "AG frag error" << std::endl;
    }
    else {
        std::cout << std::endl << "AG frag loaded" << std::endl;
    }

    VkShaderModule AGVertShader;
	if (!load_shader_module("./shaders/DiS.vert.spv", &AGVertShader))
	{
		std::cout << "Error when building the AG vertex shader module" << std::endl;
	}
	else {
		std::cout << "AG vertex shader successfully loaded" << std::endl;
	}

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, AGVertShader));

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, AGFragShader));

    pipelineBuilder._pipelineLayout = _AGPipelineLayout;

	_AGPipeline = pipelineBuilder.build_pipeline(_device, _renderPass);

    // ----------------------------------------

    // VkPipelineLayoutCreateInfo Image_pipeline_layout_info = vkinit::pipeline_layout_create_info();

    // VkPushConstantRange AGpush_constant;
	// AGpush_constant.offset = 0;
	// AGpush_constant.size = sizeof(AGPushConstants);
	// AGpush_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	// AG_pipeline_layout_info.pPushConstantRanges = &AGpush_constant;
	// AG_pipeline_layout_info.pushConstantRangeCount = 1;

    // VK_CHECK(vkCreatePipelineLayout(_device, &AG_pipeline_layout_info, nullptr, &_AGPipelineLayout));

    // VertexInputDescription AGvertexDescription = Vertex::getAG_vertex_description();

	// pipelineBuilder._vertexInputInfo.pVertexAttributeDescriptions = AGvertexDescription.attributes.data();
	// pipelineBuilder._vertexInputInfo.vertexAttributeDescriptionCount = AGvertexDescription.attributes.size();

	// pipelineBuilder._vertexInputInfo.pVertexBindingDescriptions = AGvertexDescription.bindings.data();
	// pipelineBuilder._vertexInputInfo.vertexBindingDescriptionCount = AGvertexDescription.bindings.size();

	// pipelineBuilder._shaderStages.clear();

    // VkShaderModule AGFragShader;
    // if (!load_shader_module("./shaders/DiS.frag.spv", &AGFragShader))
    // {
    //     std::cout << std::endl << "AG frag error" << std::endl;
    // }
    // else {
    //     std::cout << std::endl << "AG frag loaded" << std::endl;
    // }

    // VkShaderModule AGVertShader;
	// if (!load_shader_module("./shaders/DiS.vert.spv", &AGVertShader))
	// {
	// 	std::cout << "Error when building the AG vertex shader module" << std::endl;
	// }
	// else {
	// 	std::cout << "AG vertex shader successfully loaded" << std::endl;
	// }

	// pipelineBuilder._shaderStages.push_back(
	// 	vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, AGVertShader));

	// pipelineBuilder._shaderStages.push_back(
	// 	vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, AGFragShader));

    // pipelineBuilder._pipelineLayout = _AGPipelineLayout;

	// _AGPipeline = pipelineBuilder.build_pipeline(_device, _renderPass);

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

	pipelineBuilder._vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
	pipelineBuilder._vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.attributes.size();

	pipelineBuilder._vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
	pipelineBuilder._vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.bindings.size();

	pipelineBuilder._shaderStages.clear();

    VkShaderModule meshVertShader;
	if (!load_shader_module("./shaders/PMtriangle.vert.spv", &meshVertShader))
	{
		std::cout << "Error when building the triangle vertex shader module" << std::endl;
	}
	else {
		std::cout << "Red Triangle vertex shader successfully loaded" << std::endl;
	}

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, meshVertShader));

	pipelineBuilder._shaderStages.push_back(
		vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, pinkTriangleFragShader));

    pipelineBuilder._pipelineLayout = _meshPipelineLayout;

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

void VentumEngine::load_meshes()
{
    DEBUG("started loading meshes");
    _triangleMesh._vertices.resize(3);

	_triangleMesh._vertices[0].position = { 1.f,1.f, 0.5f };
	_triangleMesh._vertices[1].position = { -1.f,1.f, 0.5f };
	_triangleMesh._vertices[2].position = { 0.f,-1.f, 0.5f };

	_triangleMesh._vertices[0].color = { 0.f,1.f, 0.0f };
	_triangleMesh._vertices[1].color = { 0.f,1.f, 0.0f }; 
	_triangleMesh._vertices[2].color = { 0.f,1.f, 0.0f };


	_DUCKMesh.load_from_obj("./graphics/models/DUCK.obj");


    upload_mesh(_triangleMesh);
	upload_mesh(_DUCKMesh);

	_meshes["DUCK"] = _DUCKMesh;
	_meshes["triangle"] = _triangleMesh;
}

void VentumEngine::load_AG()
{
    DEBUG("started loading AG");

    FILE * MKTFILE = fopen("./graphics/ArrayGraphics/00_DEFAULT_AG.MKTI","rb");

    const char fileNames[32] = "./graphics/ArrayGraphics/.MKTAG";

    if(MKTFILE != NULL)
    {
        fseek(MKTFILE, 0L, SEEK_END);
        size_t fileSize = ftell(MKTFILE);
        rewind(MKTFILE);
        char * charArray = (char*)malloc(fileSize);
        fread( charArray,1, fileSize, MKTFILE );
        fclose( MKTFILE );

        for(int i = 0; i < fileSize; i++)
        {
            if(charArray[i-1] == '\n' && charArray[i] != '{')
            {
                int sizeOfName = 0;
                char * MKTAGName;
                for(0; charArray[i+sizeOfName] != '\n'&& sizeOfName <= fileSize-i;sizeOfName++)0;
                sizeOfName++;
                MKTAGName = (char*)malloc(sizeOfName+30);
                for(int a = 0; a < sizeOfName;a++)
                    MKTAGName[a+25] = charArray[i+a];
                for(int a = 0; a < 25;a++)
                    MKTAGName[a] = fileNames[a];
                for(int a = 0; a < 7;a++)
                    MKTAGName[sizeOfName+a-2+25] = fileNames[a+25];

                if(DEBUG("Loaded an AG: "))
                    printf("%s\n",MKTAGName);
                _HexAg = arrayGraphicsReader(MKTAGName);
                _TAGA.push_back(_HexAg);
                upload_AG(_TAGA[_TAGA.size()-1]);
            }
        }
    } else {
        printf("\nCannot find the 00_DEFAULT_AG.MKTI");
    }

    DEBUG("finished loading AG");
}

void VentumEngine::loadMenuAG()
{
    const char fileNames[19] = "./lib/icons/.MKTAG";


    // #ifdef _WIN32
    // 15;
    // #elif __gnu_linux__
    // 16;
    // #endif

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
        for(0; charArray[i+sizeOfName] != '\n' && i+sizeOfName < sizeOfFile;sizeOfName++)printf("\nbut why thought %d",sizeOfName);
        DEBUG("Read the Icon AG4");
            MKTAGName = (char*)malloc(sizeOfName+19);
        DEBUG("Read the Icon AG4");
                printf("\nlast1: %s\n",MKTAGName);
            for(int a = 0; a < sizeOfName;a++)
                MKTAGName[a+12] = charArray[a+i];
                printf("\nlast2: %s\n",MKTAGName);
            for(int a = 0; a < 12;a++)
                MKTAGName[a] = fileNames[a];
                printf("\nlast3: %s\n",MKTAGName);
            for(int a = 0; a < 7;a++)
                MKTAGName[sizeOfName+12+a] = fileNames[a+12];
                printf("\nlast: %s\n",MKTAGName);
            Modules[moduleNumber].icon.AG = arrayGraphicsReader(MKTAGName);
            // for(int a = 0; a < 3;a++)
                // Modules[moduleNumber].icon.AG._vertices[a].color = {1.0,1.0,1.0};
            AGPushConstants DEFAULTconstants;
            DEFAULTconstants.colourModification = {1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f}; // black
            float ratio = _windowExtent.width;
            ratio /= _windowExtent.height;
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
            moduleNumber++;
            free(MKTAGName);
        }
    }
    if(moduleNumber != sizeOfModules)
        0; // error, list of modules was changed whilst loading AGs
    DEBUG("Read the Icon AG");
    upload_AG(Modules[moduleNumber-1].icon.AG);
    DEBUG("Loaded an Icon AG");
}
    

void VentumEngine::init_scene()
{
    testLogo = MKTPicReader("./graphics/MKTPhotos/logo.MKTP");

    RenderObject DUCK;
    AGMaterial = {_AGPipeline,_AGPipelineLayout};
	DUCK.mesh = get_mesh("DUCK");
	DUCK.material = get_material("defaultmesh");
	DUCK.transformMatrix = glm::mat4{ 1.0f };

	_renderables.push_back(DUCK);

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
    float ratio = _windowExtent.width;
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
}

void VentumEngine::init_descriptors()
{
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
	pool_info.maxSets = 10;
	pool_info.poolSizeCount = (uint32_t)sizes.size();
	pool_info.pPoolSizes = sizes.data();

	vkCreateDescriptorPool(_device, &pool_info, nullptr, &_descriptorPool);

    for (int i = 0; i < FRAME_OVERLAP; i++)
	{
		_frames[i].cameraBuffer = create_buffer(sizeof(GPUCameraData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

		VkDescriptorSetAllocateInfo allocInfo ={};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &_globalSetLayout;

		vkAllocateDescriptorSets(_device, &allocInfo, &_frames[i].globalDescriptor);

        VkDescriptorBufferInfo binfo;
		binfo.buffer = _frames[i].cameraBuffer._buffer;
		binfo.offset = 0;
		binfo.range = sizeof(GPUCameraData);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = _pictures[0].textureImageView;
        imageInfo.sampler = _textureSampler;

		VkWriteDescriptorSet * setWrite = (VkWriteDescriptorSet*)malloc(sizeof(VkWriteDescriptorSet)*2);
		setWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        setWrite[0].dstSet = _frames[i].globalDescriptor;
        setWrite[0].dstBinding = 0;
        setWrite[0].dstArrayElement = 0;
        setWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        setWrite[0].descriptorCount = 1;
        setWrite[0].pBufferInfo = &binfo;
        setWrite[0].pNext = nullptr;

        setWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        setWrite[1].dstSet = _frames[i].globalDescriptor;
        setWrite[1].dstBinding = 1;
        setWrite[1].dstArrayElement = 0;
        setWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        setWrite[1].descriptorCount = 1;
        setWrite[1].pImageInfo = &imageInfo;
        setWrite[1].pNext = nullptr;


		vkUpdateDescriptorSets(_device, 2, setWrite, 0, nullptr);
	}

    _mainDeletionQueue.push_function([&]() {
		vkDestroyDescriptorSetLayout(_device, _globalSetLayout, nullptr);
		vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
	});

	for (int i = 0; i < FRAME_OVERLAP; i++)
	{
		_mainDeletionQueue.push_function([&]() {
			vmaDestroyBuffer(_allocator, _frames[i].cameraBuffer._buffer, _frames[i].cameraBuffer._allocation);
		});
	}
}


void VentumEngine::immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function)
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

void VentumEngine::upload_AG(MKTAG& AG)
{
    const size_t bufferSize= AG._vertices.size() * sizeof(MKTAGA);
	VkBufferCreateInfo stagingBufferInfo = {};
	stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferInfo.pNext = nullptr;
	stagingBufferInfo.size = bufferSize;
	stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;


	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

	AllocatedBuffer stagingBuffer;
    DEBUG("test1");

	VK_CHECK(vmaCreateBuffer(_allocator, &stagingBufferInfo, &vmaallocInfo,
		&stagingBuffer._buffer,
		&stagingBuffer._allocation,
		nullptr));

    DEBUG("test2");
	void* data;
	vmaMapMemory(_allocator, stagingBuffer._allocation, &data);

	memcpy(data, AG._vertices.data(), AG._vertices.size() * sizeof(MKTAGA));

    DEBUG("test4");

	vmaUnmapMemory(_allocator, stagingBuffer._allocation);


	VkBufferCreateInfo vertexBufferInfo = {};
	vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vertexBufferInfo.pNext = nullptr;
	vertexBufferInfo.size = bufferSize;
	vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	VK_CHECK(vmaCreateBuffer(_allocator, &vertexBufferInfo, &vmaallocInfo,
		&AG._vertexBuffer._buffer,
		&AG._vertexBuffer._allocation,
		nullptr));

	immediate_submit([=](VkCommandBuffer cmd) {
		VkBufferCopy copy;
		copy.dstOffset = 0;
		copy.srcOffset = 0;
		copy.size = bufferSize;
		vkCmdCopyBuffer(cmd, stagingBuffer._buffer, AG._vertexBuffer._buffer, 1, & copy);
	});

    _mainDeletionQueue.push_function([=]() {
		vmaDestroyBuffer(_allocator, AG._vertexBuffer._buffer, AG._vertexBuffer._allocation);
	});

	vmaDestroyBuffer(_allocator, stagingBuffer._buffer, stagingBuffer._allocation);
}

void VentumEngine::init_createTextureSampler()
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

void VentumEngine::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
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

void VentumEngine::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
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

void VentumEngine::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

VkCommandBuffer VentumEngine::beginSingleTimeCommands() {


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

void VentumEngine::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_graphicsQueue);

    vkFreeCommandBuffers(_device, get_current_frame()._commandPool, 1, &commandBuffer);
}

void VentumEngine::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
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

void VentumEngine::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK(vkCreateBuffer(_device, &bufferInfo, nullptr, &buffer));

        DEBUG("created a buffer");
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if(allocInfo.memoryTypeIndex == -1)
        DEBUG("and we have a problem on our hands");

    VK_CHECK(vkAllocateMemory(_device, &allocInfo, nullptr, &bufferMemory));
        DEBUG("allocated memory");

    vkBindBufferMemory(_device, buffer, bufferMemory, 0);
}

int VentumEngine::loading_MKTP_image(MKTPic data)
{
    VkDeviceSize imageSize = data.width * data.height * 4;

    VkBuffer stagingBuffer;

    // createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    VkBufferCreateInfo stagingBufferInfo = {};
	stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferInfo.pNext = nullptr;
	stagingBufferInfo.size = imageSize;
	stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    AllocatedBuffer stagingBufferMemory;

    VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY; // VMA_MEMORY_USAGE_CPU_TO_GPU // VMA_MEMORY_USAGE_AUTO

	VK_CHECK(vmaCreateBuffer(_allocator, &stagingBufferInfo, &vmaallocInfo,
		&stagingBufferMemory._buffer,
		&stagingBufferMemory._allocation,
		nullptr));

    DEBUG("created buffer");
    void* data_ptr;
    vmaMapMemory(_allocator, stagingBufferMemory._allocation, &data_ptr);
	memcpy(data_ptr, data.pixels, imageSize);
	vmaUnmapMemory(_allocator, stagingBufferMemory._allocation);
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
    copyBufferToImage(stagingBufferMemory._buffer, textureImage, static_cast<uint32_t>(data.width), static_cast<uint32_t>(data.height));

    DEBUG("copied buffer to image");
    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(_device, stagingBufferMemory._buffer, nullptr);
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

    _pictures.resize(_pictures.size()+1);
    _pictures[_pictures.size()-1] = {textureImage,textureImageMemory,textureImageView};

    _mainDeletionQueue.push_function([=]() {
        vkDestroyImageView(_device, _pictures[_pictures.size()-1].textureImageView, nullptr);
        vkDestroyImage(_device, _pictures[_pictures.size()-1].textureImage, nullptr);
        vkFreeMemory(_device, _pictures[_pictures.size()-1].textureImageMemory, nullptr);
    });

    return _pictures.size()-1;
    // some shananigans
}

uint32_t VentumEngine::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_chosenGPU, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i)) {
            return i;
        }
    }
    // error checking would be nice (memory type not found)
    return -1;
}

void VentumEngine::upload_mesh(Mesh& mesh)
{
    const size_t bufferSize= mesh._vertices.size() * sizeof(Vertex);
	VkBufferCreateInfo stagingBufferInfo = {};
	stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferInfo.pNext = nullptr;
	stagingBufferInfo.size = bufferSize;
	stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;


	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

	AllocatedBuffer stagingBuffer;

	VK_CHECK(vmaCreateBuffer(_allocator, &stagingBufferInfo, &vmaallocInfo,
		&stagingBuffer._buffer,
		&stagingBuffer._allocation,
		nullptr));	

	void* data;
	vmaMapMemory(_allocator, stagingBuffer._allocation, &data);

	memcpy(data, mesh._vertices.data(), mesh._vertices.size() * sizeof(Vertex));

	vmaUnmapMemory(_allocator, stagingBuffer._allocation);


	VkBufferCreateInfo vertexBufferInfo = {};
	vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vertexBufferInfo.pNext = nullptr;
	vertexBufferInfo.size = bufferSize;
	vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	VK_CHECK(vmaCreateBuffer(_allocator, &vertexBufferInfo, &vmaallocInfo,
		&mesh._vertexBuffer._buffer,
		&mesh._vertexBuffer._allocation,
		nullptr));

	immediate_submit([=](VkCommandBuffer cmd) {
		VkBufferCopy copy;
		copy.dstOffset = 0;
		copy.srcOffset = 0;
		copy.size = bufferSize;
		vkCmdCopyBuffer(cmd, stagingBuffer._buffer, mesh._vertexBuffer._buffer, 1, & copy);
	});

    _mainDeletionQueue.push_function([=]() {
		vmaDestroyBuffer(_allocator, mesh._vertexBuffer._buffer, mesh._vertexBuffer._allocation);
	});

	vmaDestroyBuffer(_allocator, stagingBuffer._buffer, stagingBuffer._allocation);
}

Material* VentumEngine::create_material(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name)
{
	Material mat;
	mat.pipeline = pipeline;
	mat.pipelineLayout = layout;
	_materials[name] = mat;
	return &_materials[name];
}

Material* VentumEngine::get_material(const std::string& name)
{
	auto it = _materials.find(name);
	if (it == _materials.end()) {
		return nullptr;
	}
	else {
		return &(*it).second;
	}
}


Mesh* VentumEngine::get_mesh(const std::string& name)
{
	auto it = _meshes.find(name);
	if (it == _meshes.end()) {
		return nullptr;
	}
	else {
		return &(*it).second;
	}
}

void VentumEngine::drawMenu(VkCommandBuffer cmd,GL * menuStuff, size_t sizeOfMenuStuff)
{
	for (int i = 0; i < sizeOfMenuStuff; i++) // make it less than 18 lines per draw
	{
        if(Modules[i].icon.isVisible)
        {
            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _AGPipeline);

            Modules[i].icon.AGPC.movement.y = CBT+i/20;

            VkDeviceSize offset = 0;
            vkCmdBindVertexBuffers(cmd, 0, 1, &Modules[i].icon.AG._vertexBuffer._buffer, &offset);

            vkCmdPushConstants(cmd, _AGPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &Modules[i].icon.AGPC);

            vkCmdDraw(cmd, Modules[i].icon.AG._vertices.size(), 1, 0, 0);
        }
    }

    for(int i = 0; i < sizeOfMenuStuff+5;i++) // make it less than 18 lines per draw
    { 
        if(Modules[i].icon.isVisible) {

            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _AGPipeline);

            VkDeviceSize offset = 0;
            vkCmdBindVertexBuffers(cmd, 0, 1, &_AGA[BLACKLINE_ID].AG._vertexBuffer._buffer, &offset);

            AGPushConstants BLC = _AGA[0].AGPC;

            BLC.movement.y = CBT+i/20;
            
            vkCmdPushConstants(cmd, _AGPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &BLC);

            vkCmdDraw(cmd, _AGA[BLACKLINE_ID].AG._vertices.size(), 1, 0, 0);
        }
    }

}

void VentumEngine::draw_AG(VkCommandBuffer cmd,sub2MAKiT* first, int count)
{
	for (int i = 0; i < count; i++)
	{
        if(first[i].isVisible)
        {
            AGPushConstants constants = first[i].AGPC;
            MKTAG * object = &first[i].AG;

            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _AGPipeline);

            // vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _AGPipelineLayout, 0, 1, &get_current_frame().globalDescriptor, 0, nullptr);

            VkDeviceSize offset = 0;

            vkCmdPushConstants(cmd, _AGPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &constants);

            vkCmdBindVertexBuffers(cmd, 0, 1, &object->_vertexBuffer._buffer, &offset);

            vkCmdDraw(cmd, object->_vertices.size(), 1, 0, 0);
        }
    }
}

void VentumEngine::draw_objects(VkCommandBuffer cmd,RenderObject* first, int count)
{
	glm::vec3 camPos = { 0.f,-6.f,-10.f };

	glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
	glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
	projection[1][1] *= -1;

    GPUCameraData camData;
	camData.proj = projection;
	camData.view = view;
	camData.viewproj = projection * view;

	void* data;
	vmaMapMemory(_allocator, get_current_frame().cameraBuffer._allocation, &data);

	memcpy(data, &camData, sizeof(GPUCameraData));

	vmaUnmapMemory(_allocator, get_current_frame().cameraBuffer._allocation);

	Mesh* lastMesh = nullptr;
	Material* lastMaterial = nullptr;
	for (int i = 0; i < count; i++)
	{
		RenderObject& object = first[i];

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
			vkCmdBindVertexBuffers(cmd, 0, 1, &object.mesh->_vertexBuffer._buffer, &offset);
			lastMesh = object.mesh;
		}
		vkCmdDraw(cmd, object.mesh->_vertices.size(), 1, 0, 0);
	}
}

FrameData& VentumEngine::get_current_frame()
{
	return _frames[_frameNumber % FRAME_OVERLAP];
}

AllocatedBuffer VentumEngine::create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage)
{
	//allocate vertex buffer
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = nullptr;

	bufferInfo.size = allocSize;
	bufferInfo.usage = usage;


	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = memoryUsage;

	AllocatedBuffer newBuffer;

	//allocate the buffer
	VK_CHECK(vmaCreateBuffer(_allocator, &bufferInfo, &vmaallocInfo,
		&newBuffer._buffer,
		&newBuffer._allocation,
		nullptr));

	return newBuffer;
}