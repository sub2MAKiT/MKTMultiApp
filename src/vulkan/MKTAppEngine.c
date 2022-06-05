#define MAIN_MKT_APP_ENGINE_INCLUDE
#define VMA_IMPLEMENTATION
#include "MKTAppEngine.h"
#include <MKTmisc/MKTGNUbasedHeaders/MKTmath.h>
#include <iostream>

#include <SDL/SDL.h>
#include<SDL/SDL_vulkan.h>

#include <fstream>

#include <vk-bootstrap/src/VkBootstrap.h>

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
printf_s("\033[%d;40m[%d]" x "\033[0;40m\n",int((rand() % 6) + 1 + MKTfloor(float(((rand() % 2) + 1)*1.5)) * 30),step);step++
#else
#define DEBUG(x)                                                \
0
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

    _MKTGE_load_meshes();

    _MKTGE_load_AG();

    _isInitialized = true;
}

void _MKTGE_cleanup() {
    if (_isInitialized) {

        vkWaitForFences(_device, 1, &_renderFence, true, 1000000000);

        _mainDeletionQueue.flush();

        vkDestroySurfaceKHR(_instance, _surface, NULL);

        vkDestroyDevice(_device, NULL);
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

vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _meshPipeline);

VkDeviceSize offset = 0;
vkCmdBindVertexBuffers(cmd,0,1,&_triangleMesh._vertexBuffer._buffer,&offset);

vkCmdDraw(cmd, _triangleMesh.sizeOfArray, 1, 0, 0);

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
            if (e.type == SDL_QUIT)
                bQuit = true;
            else if(e.type == SDL_KEYDOWN)
                if(e.key.keysym.sym == SDLK_SPACE)
                {
                    _selectedShader += 1 - _selectedShader*2;
                }
        }
    }
    DEBUG("III ran III");
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

void _MKTGE_init_vulkan() //#0000ff
{
    vkb::InstanceBuilder builder;

    auto inst_ret = builder.set_app_name("MKTMultiApp")
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

        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.physicalDevice = _chosenGPU;
        allocatorInfo.device = _device;
        allocatorInfo.instance = _instance;
        vmaCreateAllocator(&allocatorInfo, &_allocator);

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

        _mainDeletionQueue.push_function([=]() {
		vkDestroySwapchainKHR(_device, _swapchain, nullptr);
	    });

    DEBUG("II init:swapchain II");
}

void _MKTGE_init_commands()
{
    VkCommandPoolCreateInfo commandPoolInfo = command_pool_create_info(_graphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    VK_CHECK(vkCreateCommandPool(_device, &commandPoolInfo, NULL, &_commandPool));

    VkCommandBufferAllocateInfo cmdAllocInfo = command_buffer_allocate_info(_commandPool,1,VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    VK_CHECK(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &_mainCommandBuffer));

    _mainDeletionQueue.push_function([=]() {
		vkDestroyCommandPool(_device, _commandPool, nullptr);
	});

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

    _mainDeletionQueue.push_function([=]() {
		vkDestroyRenderPass(_device, _renderPass, nullptr);
    });

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
        _mainDeletionQueue.push_function([=]() {
			vkDestroyFramebuffer(_device, _framebuffers[i], nullptr);
			vkDestroyImageView(_device, _swapchainImageViews[i], nullptr);
    	});
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

    _mainDeletionQueue.push_function([=]() {
        vkDestroyFence(_device, _renderFence, NULL);
        vkDestroySemaphore(_device, _presentSemaphore, NULL);
        vkDestroySemaphore(_device, _renderSemaphore, NULL);
    });

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
  
    DEBUG("got vertex descriptions");

    VertexInputDescription vertexDescription = get_vertex_description();

    _vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings;
    _vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes;

	_vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.sizeOfArrayA;
	_vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.sizeOfArrayB;

    VkShaderModule CtriangleFragShader;
    if (!_MKTGE_load_shader_module("./build/shaders/Ctriangle.frag.spv", &CtriangleFragShader))
    {
        DEBUG("Ctriangle frag error");
    }
    else {
        DEBUG("Ctriangle frag loaded");
    }

    VkShaderModule CtriangleVertexShader;
    if (!_MKTGE_load_shader_module("./build/shaders/Ctriangle.vert.spv", &CtriangleVertexShader))
    {
        DEBUG("triangle vert error");
    }
    else {
        DEBUG("triangle vert loaded");
    }

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

    _shaderStages.clear();

    _shaderStages.push_back(
        pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, CtriangleVertexShader)
    );

    _shaderStages.push_back(
        pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, CtriangleFragShader)
    );

    _CtrianglePipeline = build_pipeline(_device, _renderPass);

	_shaderStages.clear();

    _vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings;
    _vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes;

	_vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.sizeOfArrayA;
	_vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.sizeOfArrayB;

	VkShaderModule meshVertShader;
	if(!_MKTGE_load_shader_module("./build/shaders/PMtriangle.vert.spv", &meshVertShader))
    {
        DEBUG("Error when building the triangle vertex shader module");
    } else {
        DEBUG("PM Triangle vertex shader successfully loaded");
    }

	_shaderStages.push_back(
		pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, meshVertShader));

	_shaderStages.push_back(
		pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, CtriangleFragShader));

	_meshPipeline = build_pipeline(_device, _renderPass);

	vkDestroyShaderModule(_device, meshVertShader, NULL);
	vkDestroyShaderModule(_device, CtriangleVertexShader, NULL);
	vkDestroyShaderModule(_device, CtriangleFragShader, NULL);
	vkDestroyShaderModule(_device, triangleFragShader, NULL);
	vkDestroyShaderModule(_device, triangleVertexShader, NULL);

	_mainDeletionQueue.push_function([=]() {
		vkDestroyPipeline(_device, _CtrianglePipeline,NULL);
		vkDestroyPipeline(_device, _trianglePipeline, NULL);
		vkDestroyPipeline(_device, _meshPipeline, NULL);

		vkDestroyPipelineLayout(_device, _trianglePipelineLayout, NULL);
	});
}

void _MKTGE_load_meshes()
{
    int numberOfV = 3;


    if(DEBUG("check:"))
        printf("%d times %d should be equal to %d\n",sizeof(MKTDOUBLE),9*numberOfV,sizeof(MKTGraphics3)*numberOfV);

	_triangleMesh._verticies = (MKTGraphics3*)malloc(sizeof(MKTGraphics3)*numberOfV);
    // for(long long i = 0; i < numberOfV; i++)
    

    _triangleMesh._verticies[0].position.value[0] = 1.0;
    _triangleMesh._verticies[0].position.value[1] = 1.0;
    _triangleMesh._verticies[0].position.value[2] = 0.0;

    _triangleMesh._verticies[1].position.value[0] = -1.f;
    _triangleMesh._verticies[1].position.value[1] = 1.f;
    _triangleMesh._verticies[1].position.value[2] = 0.0f;

    _triangleMesh._verticies[2].position.value[0] = 0.f;
    _triangleMesh._verticies[2].position.value[1] = -1.f;
    _triangleMesh._verticies[2].position.value[2] = 0.0f;

	_triangleMesh._verticies[0].colour.value[0] = 0.f;
	_triangleMesh._verticies[0].colour.value[1] = 1.f;
	_triangleMesh._verticies[0].colour.value[2] = 0.0f;

	_triangleMesh._verticies[1].colour.value[0] = 0.f;
	_triangleMesh._verticies[1].colour.value[1] = 1.f;
	_triangleMesh._verticies[1].colour.value[2] = 0.0f;
	
    _triangleMesh._verticies[2].colour.value[0] = 0.f;
    _triangleMesh._verticies[2].colour.value[1] = 1.f;
    _triangleMesh._verticies[2].colour.value[2] = 0.0f;
    
	_triangleMesh._verticies[0].normals.value[0] = 0.f;
	_triangleMesh._verticies[0].normals.value[1] = 0.f;
	_triangleMesh._verticies[0].normals.value[2] = 0.f;

	_triangleMesh._verticies[1].normals.value[0] = 0.f;
	_triangleMesh._verticies[1].normals.value[1] = 0.f;
	_triangleMesh._verticies[1].normals.value[2] = 0.f;

	_triangleMesh._verticies[2].normals.value[0] = 0.f;
	_triangleMesh._verticies[2].normals.value[1] = 0.f;
	_triangleMesh._verticies[2].normals.value[2] = 0.f;

    _triangleMesh.sizeOfArray = numberOfV;

	_MKTGE_upload_mesh(&_triangleMesh);
    return;
}

void _MKTGE_load_AG()
{
    int numberOfV = 3;
	_CtriangleMesh._verticies = (MKTGraphics2*)malloc(sizeof(MKTGraphics2)*numberOfV);

    _CtriangleMesh._verticies[0].position.value[0] = 1.f;
    _CtriangleMesh._verticies[0].position.value[1] = 1.f;
    _CtriangleMesh._verticies[0].position.value[2] = 0.0f;

    _CtriangleMesh._verticies[1].position.value[0] = -1.f;
    _CtriangleMesh._verticies[1].position.value[1] = 1.f;
    _CtriangleMesh._verticies[1].position.value[2] = 0.0f;

    _CtriangleMesh._verticies[2].position.value[0] = 0.f;
    _CtriangleMesh._verticies[2].position.value[1] = -1.f;
    _CtriangleMesh._verticies[2].position.value[2] = 0.0f;

	_CtriangleMesh._verticies[0].colour.value[0] = 1.f;
	_CtriangleMesh._verticies[0].colour.value[1] = 0.f;
	_CtriangleMesh._verticies[0].colour.value[2] = 0.0f;

	_CtriangleMesh._verticies[1].colour.value[0] = 0.f;
	_CtriangleMesh._verticies[1].colour.value[1] = 1.f;
	_CtriangleMesh._verticies[1].colour.value[2] = 0.0f;
	
    _CtriangleMesh._verticies[2].colour.value[0] = 0.f;
    _CtriangleMesh._verticies[2].colour.value[1] = 0.f;
    _CtriangleMesh._verticies[2].colour.value[2] = 1.0f;

    _CtriangleMesh.sizeOfArray = numberOfV;

	_MKTGE_upload_AG(&_CtriangleMesh);
    return;
}

void _MKTGE_upload_mesh(Mesh * mesh)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = mesh[0].sizeOfArray * sizeof(MKTGraphics3);
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

	VK_CHECK(vmaCreateBuffer(_allocator, &bufferInfo, &vmaallocInfo,
		&mesh->_vertexBuffer._buffer,
		&mesh->_vertexBuffer._allocation,
		nullptr));

	_mainDeletionQueue.push_function([=]() {
        vmaDestroyBuffer(_allocator, mesh[0]._vertexBuffer._buffer, mesh[0]._vertexBuffer._allocation);
    });

    void* data;

    vmaMapMemory(_allocator, mesh[0]._vertexBuffer._allocation, &data);

    data = malloc(mesh[0].sizeOfArray * sizeof(MKTGraphics3)); // this may not be the best solution, but (insert something smart here)
    void * pointerHack = mesh[0]._verticies; // step 1, a cool reference
    for(long long i = 0; i < mesh[0].sizeOfArray * sizeof(MKTGraphics3);i++) // step 2, loop
        *(char*)((char*)data + i) = *(char*)((char*)pointerHack + i); // wtf even is this?

	vmaUnmapMemory(_allocator, mesh[0]._vertexBuffer._allocation); // step 3 done

    return;
}

void _MKTGE_upload_AG(arrayGraphic * AG)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = AG[0].sizeOfArray * sizeof(MKTGraphics2);
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

	VK_CHECK(vmaCreateBuffer(_allocator, &bufferInfo, &vmaallocInfo,
		&AG->_vertexBuffer._buffer,
		&AG->_vertexBuffer._allocation,
		nullptr));

	_mainDeletionQueue.push_function([=]() {
        vmaDestroyBuffer(_allocator, AG[0]._vertexBuffer._buffer, AG[0]._vertexBuffer._allocation);
    });

    void* data;

    vmaMapMemory(_allocator, AG[0]._vertexBuffer._allocation, &data);

    data = malloc(AG[0].sizeOfArray * sizeof(MKTGraphics2)); // this may not be the best solution, but (insert something smart here)
    void * pointerHack = AG[0]._verticies; // step 1, a cool reference
    for(long long i = 0; i < AG[0].sizeOfArray * sizeof(MKTGraphics2);i++) // step 2, loop
        *(char*)((char*)data + i) = *(char*)((char*)pointerHack + i); // wtf even is this?

	vmaUnmapMemory(_allocator, AG[0]._vertexBuffer._allocation); // step 3 done

    return;
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

    VkGraphicsPipelineCreateInfo pipelineInfo = {}; //#ff0000
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = NULL;

    pipelineInfo.stageCount = _shaderStages.size();
    pipelineInfo.pStages = _shaderStages.data();
    pipelineInfo.pVertexInputState = &_vertexInputInfo; //#ff0000
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
    if (vkCreateGraphicsPipelines( //#ff0000
        _device, VK_NULL_HANDLE, 1, &pipelineInfo , NULL, &newPipeline) != VK_SUCCESS) {
            DEBUG("failed to create pipeline\n");
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

VkFenceCreateInfo fence_create_info(VkFenceCreateFlags flags)
{
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = flags;
    return fenceCreateInfo;
}

VkSemaphoreCreateInfo semaphore_create_info(VkSemaphoreCreateFlags flags)
{
    VkSemaphoreCreateInfo semCreateInfo = {};
    semCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semCreateInfo.pNext = nullptr;
    semCreateInfo.flags = flags;
    return semCreateInfo;
}


VertexInputDescription get_vertex_description()
{
	VkVertexInputBindingDescription mainBinding = {};
	mainBinding.binding = 0;
	mainBinding.stride = sizeof(MKTGraphics3);
	mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    size_t sizeOfArrayB = 1;
    VkVertexInputBindingDescription * tempBindPoint = (VkVertexInputBindingDescription*)malloc(sizeof(VkVertexInputBindingDescription)*sizeOfArrayB);
    tempBindPoint[0] = mainBinding;

	VkVertexInputAttributeDescription positionAttribute = {};
	positionAttribute.binding = 0;
	positionAttribute.location = 0;
	positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	positionAttribute.offset = offsetof(MKTGraphics3, position);

	VkVertexInputAttributeDescription normalAttribute = {};
	normalAttribute.binding = 0;
	normalAttribute.location = 1;
	normalAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	normalAttribute.offset = offsetof(MKTGraphics3, normals);

	VkVertexInputAttributeDescription colorAttribute = {};
	colorAttribute.binding = 0;
	colorAttribute.location = 2;
	colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	colorAttribute.offset = offsetof(MKTGraphics3, colour);

    size_t sizeOfArrayA = 3;
    VkVertexInputAttributeDescription * tempAttrPoint = (VkVertexInputAttributeDescription*)malloc(sizeof(VkVertexInputAttributeDescription)*sizeOfArrayA);
    tempAttrPoint[0] = positionAttribute;
    tempAttrPoint[1] = normalAttribute;
	tempAttrPoint[2] = colorAttribute;

	VertexInputDescription description = {tempBindPoint, tempAttrPoint, sizeOfArrayB, sizeOfArrayA, 0};
	return description;
}

VertexInputDescription get_AGvertex_description()
{

	VkVertexInputBindingDescription mainBinding = {};
	mainBinding.binding = 0;
	mainBinding.stride = sizeof(MKTGraphics2);
	mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    size_t sizeOfArrayB = 1;
    VkVertexInputBindingDescription * tempBindPoint = (VkVertexInputBindingDescription*)malloc(sizeof(VkVertexInputBindingDescription)*sizeOfArrayB);
    tempBindPoint[0] = mainBinding;


	VkVertexInputAttributeDescription positionAttribute = {};
	positionAttribute.binding = 0;
	positionAttribute.location = 0;
	positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	positionAttribute.offset = offsetof(MKTGraphics2, position);

	VkVertexInputAttributeDescription colorAttribute = {};
	colorAttribute.binding = 0;
	colorAttribute.location = 1;
	colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	colorAttribute.offset = offsetof(MKTGraphics2, colour);

    size_t sizeOfArrayA = 2;
    VkVertexInputAttributeDescription * tempAttrPoint = (VkVertexInputAttributeDescription*)malloc(sizeof(VkVertexInputAttributeDescription)*sizeOfArrayA);
    tempAttrPoint[0] = positionAttribute;
	tempAttrPoint[1] = colorAttribute;

	VertexInputDescription description = {tempBindPoint, tempAttrPoint, sizeOfArrayB, sizeOfArrayA, 0};
	return description;
}