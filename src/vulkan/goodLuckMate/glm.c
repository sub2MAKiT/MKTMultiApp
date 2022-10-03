#include <goodLuckMate/glm.h>

VkPipelineVertexInputStateCreateInfo getInputDescription(IntDex i) {
    VkPipelineVertexInputStateCreateInfo returnInfo = {};
    VkVertexInputBindingDescription * bindingDescription;

    SAFEMALLOC(bindingDescription,(sizeof(VkVertexInputBindingDescription)));

    switch(i) // MKTAGVertex
    {
        case 0:
        {
            bindingDescription->binding = 0;
            bindingDescription->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            VkVertexInputAttributeDescription * attributeDescriptions;

            SAFEMALLOC(attributeDescriptions,(sizeof(VkVertexInputAttributeDescription)*2));

            bindingDescription->stride = sizeof(AGVertex);

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(AGVertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(AGVertex, colour);

            returnInfo.vertexBindingDescriptionCount = 1;
            returnInfo.vertexAttributeDescriptionCount = 2;
            returnInfo.pVertexBindingDescriptions = bindingDescription;
            returnInfo.pVertexAttributeDescriptions = attributeDescriptions;
        }
        case 1: // MKTPiCVertex
        {
            bindingDescription->binding = 0;
            bindingDescription->stride = sizeof(PiCVertex);
            bindingDescription->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            VkVertexInputAttributeDescription * attributeDescriptions;

            SAFEMALLOC(attributeDescriptions,(sizeof(VkVertexInputAttributeDescription)*3));

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(PiCVertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(PiCVertex, colour);


            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(PiCVertex, tex);

            returnInfo.vertexBindingDescriptionCount = 1;
            returnInfo.vertexAttributeDescriptionCount = 3;
            returnInfo.pVertexBindingDescriptions = bindingDescription;
            returnInfo.pVertexAttributeDescriptions = attributeDescriptions;
        }
        case 2:
        {
            bindingDescription->binding = 0;
            bindingDescription->stride = sizeof(PiCVertex);
            bindingDescription->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            VkVertexInputAttributeDescription * attributeDescriptions;

            SAFEMALLOC(attributeDescriptions,(sizeof(VkVertexInputAttributeDescription)*3));

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(PiCVertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(PiCVertex, colour);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(PiCVertex, tex);

            returnInfo.vertexBindingDescriptionCount = 1;
            returnInfo.vertexAttributeDescriptionCount = 3;
            returnInfo.pVertexBindingDescriptions = bindingDescription;
            returnInfo.pVertexAttributeDescriptions = attributeDescriptions;
        }
    }


    returnInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    // returnInfo.flags = 0;
    returnInfo.pNext = NULL;



    return returnInfo;
}