#include <pipeline/pipeline.h>

void _MKT_LOAD_PIPELINE(const char * FPV,const char * FPF, IntDex materialIndex)
{
    FILE *MKTFILEV = fopen(FPV,"rb");
    char *listV;
    long sizeOfFileV;
    if(MKTFILEV != NULL)
    {
    fseek(MKTFILEV, 0L, SEEK_END);
    sizeOfFileV = ftell(MKTFILEV);
    rewind(MKTFILEV);
    listV = (char*)malloc(sizeOfFileV);
    if(listV == NULL)
        MKTerror(3);
    fread( listV,1, sizeOfFileV, MKTFILEV );
    fclose( MKTFILEV );
    } else {
        MKTerror(2);
    }

    FILE *MKTFILEF = fopen(FPF,"rb");
    char *listF;
    long sizeOfFileF;
    if(MKTFILEF != NULL)
    {
    fseek(MKTFILEF, 0L, SEEK_END);
    sizeOfFileF = ftell(MKTFILEF);
    rewind(MKTFILEF);
    listF = (char*)malloc(sizeOfFileF);
    if(listF == NULL)
        MKTerror(3);
    fread( listF,1, sizeOfFileF, MKTFILEF );
    fclose( MKTFILEF );
    } else {
        MKTerror(2);
    }

    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

        listV = realloc(listV,sizeOfFileV);

        createInfo.codeSize = sizeOfFileV;
        createInfo.pCode = (unsigned int*)listV;

        VkShaderModule shaderModule;
        VK_CHECK(vkCreateShaderModule(_device, &createInfo, NULL, &shaderModule));


        _REN_materials[materialIndex].vert = shaderModule;

        DELQUEVARIABLES
        VkDevice a;VkShaderModule b;const VkAllocationCallbacks* c;
        DELQUEVARIABLESDOT(vkDestroyShaderModule)
        input->a,input->b,input->c
        DELQUEVARIABLESVALUE
        _device,_REN_materials[materialIndex].vert,NULL
        COMMITDELQUE(vkDestroyShaderModule)

        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = shaderModule;
        vertShaderStageInfo.pName = "main";

        _REN_materials[materialIndex].vertI = vertShaderStageInfo;
    }

    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

        listF = realloc(listF,sizeOfFileF);

        createInfo.codeSize = sizeOfFileF;
        createInfo.pCode = (unsigned int*)listF;

        VkShaderModule shaderModule;
        VK_CHECK(vkCreateShaderModule(_device, &createInfo, NULL, &shaderModule));

        _REN_materials[materialIndex].frag = shaderModule;

        DELQUEVARIABLES
        VkDevice a;VkShaderModule b;const VkAllocationCallbacks* c;
        DELQUEVARIABLESDOT(vkDestroyShaderModule)
        input->a,input->b,input->c
        DELQUEVARIABLESVALUE
        _device,_REN_materials[materialIndex].frag,NULL
        COMMITDELQUE(vkDestroyShaderModule)

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = shaderModule;
        fragShaderStageInfo.pName = "main";

        _REN_materials[materialIndex].fragI = fragShaderStageInfo;

    }



    IntDex sizeOfDynamicStates = 2;
    VkDynamicState * dynamicStates = malloc(sizeof(VkDynamicState)*sizeOfDynamicStates);
    dynamicStates[0] = VK_DYNAMIC_STATE_VIEWPORT;
    dynamicStates[1] = VK_DYNAMIC_STATE_SCISSOR;

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = sizeOfDynamicStates;
    dynamicState.pDynamicStates = dynamicStates;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = NULL; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = NULL; // Optional

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)_swapChainExtent.width;
    viewport.height = (float)_swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent = _swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = NULL; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = NULL; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = NULL; // Optional

    VK_CHECK(vkCreatePipelineLayout(_device, &pipelineLayoutInfo, NULL, &_REN_materials[materialIndex].pipelineLayout));

    VkPipelineShaderStageCreateInfo shaderStages[2] = {_REN_materials[materialIndex].vertI, _REN_materials[materialIndex].fragI};

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;

    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = NULL; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;

    pipelineInfo.layout = _REN_materials[materialIndex].pipelineLayout;

    pipelineInfo.renderPass = _renderPass;
    pipelineInfo.subpass = 0;

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    VK_CHECK(vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &_REN_materials[materialIndex].graphicsPipeline));

    DELQUEVARIABLES
    VkDevice a;VkPipeline b;const VkAllocationCallbacks* c;
    DELQUEVARIABLESDOT(vkDestroyPipeline)
    input->a,input->b,input->c
    DELQUEVARIABLESVALUE
    _device,_REN_materials[materialIndex].graphicsPipeline,NULL
    COMMITDELQUE(vkDestroyPipeline)


    DELQUEVARIABLES
    VkDevice a;VkPipelineLayout b;const VkAllocationCallbacks* c;
    DELQUEVARIABLESDOT(vkDestroyPipelineLayout)
    input->a,input->b,input->c
    DELQUEVARIABLESVALUE
    _device,_REN_materials[materialIndex].pipelineLayout,NULL
    COMMITDELQUE(vkDestroyPipelineLayout)

    free(listV);
    free(listF);

    return;
}