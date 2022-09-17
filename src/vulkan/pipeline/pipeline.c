#include <pipeline/pipeline.h>
#include <structs.h>

void _MKT_LOAD_DESCRIPTORS(IntDex materialIndex, IntDex glmI)
{
    printf("material: %d %d\n",materialIndex, glmI);
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    VkDescriptorSetLayoutBinding layoutBinding = {};
    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
    if(glmI == 0)// MKTAG
    {
        layoutBinding.binding = 0;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        layoutBinding.pImmutableSamplers = NULL;
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &layoutBinding;
    }
    else if(glmI == 1)// MKTPiC
    {
        layoutBinding.binding = 0;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        layoutBinding.pImmutableSamplers = NULL;

        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = NULL;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutBinding * bindings = malloc(sizeof(VkDescriptorSetLayoutBinding)*2);
        bindings[0] = layoutBinding; 
        bindings[1] = samplerLayoutBinding;
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 2;
        layoutInfo.pBindings = bindings;
    }
    VK_CHECK(vkCreateDescriptorSetLayout(_device, &layoutInfo, NULL, &_ren_materials[materialIndex].descriptorSetLayout));
    return;
}

void _MKT_LOAD_PIPELINE(const char * FPV,const char * FPF, IntDex glmI) //AG = 0
{
    _ren_sizeOfMaterials++;
    _ren_materials = realloc(_ren_materials,_ren_sizeOfMaterials*sizeof(MKTmaterial));
    IntDex materialIndex = _ren_sizeOfMaterials-1;
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

    VkShaderModule fragShaderModule;
    VkShaderModule vertShaderModule;
    VkPipelineShaderStageCreateInfo fragI;
    VkPipelineShaderStageCreateInfo vertI;

    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

        listV = realloc(listV,sizeOfFileV);

        createInfo.codeSize = sizeOfFileV;
        createInfo.pCode = (unsigned int*)listV;

        VkShaderModule shaderModule;
        VK_CHECK(vkCreateShaderModule(_device, &createInfo, NULL, &shaderModule));


        vertShaderModule = shaderModule;

        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = shaderModule;
        vertShaderStageInfo.pName = "main";

        vertI = vertShaderStageInfo;
    }

    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

        listF = realloc(listF,sizeOfFileF);

        createInfo.codeSize = sizeOfFileF;
        createInfo.pCode = (unsigned int*)listF;

        VkShaderModule shaderModule;
        VK_CHECK(vkCreateShaderModule(_device, &createInfo, NULL, &shaderModule));

        fragShaderModule = shaderModule;

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = shaderModule;
        fragShaderStageInfo.pName = "main";

        fragI = fragShaderStageInfo;

    }

    VkPipelineShaderStageCreateInfo shaderStages[2] = {vertI, fragI};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = getInputDescription(glmI);

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkDynamicState * dynamicStates = malloc(sizeof(VkDynamicState)*2);
    
    dynamicStates[0] = VK_DYNAMIC_STATE_VIEWPORT;
    dynamicStates[1] = VK_DYNAMIC_STATE_SCISSOR;

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;
    _MKT_LOAD_DESCRIPTORS(materialIndex, glmI);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &_ren_materials[materialIndex].descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    VK_CHECK(vkCreatePipelineLayout(_device, &pipelineLayoutInfo, NULL, &_ren_materials[materialIndex].pipelineLayout));

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;        
    pipelineInfo.layout = _ren_materials[materialIndex].pipelineLayout;
    pipelineInfo.renderPass = _renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    VK_CHECK(vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &_ren_materials[materialIndex].graphicsPipeline));

    vkDestroyShaderModule(_device, fragShaderModule, NULL);
    vkDestroyShaderModule(_device, vertShaderModule, NULL);




    free(listV);
    free(listF);

    return;
}