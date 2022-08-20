#define _MKTENGINEINCLUDEGUARD_
#define MKTUTILSTHIRD

#include <MKTAppEngine.h>

#ifdef _USE_GLFW
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

// Some basic graphics defines:

#define MKTDRAW3D 1
#define MKTDRAWAG 2
#define MKTDRAWPIC 4
#define MKTDRAWMENU 8
#define MKTDRAWTEXT 16

#define BLACKLINE_ID 0
#define MENUARRAYGRAPHIC 4
#define MENULINEHORIZONTAL 5

void init() {

    glfwInit();\
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);\
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);\
    _window = glfwCreateWindow(_windowExtent.width, _windowExtent.height, "Vulkan", nullptr, nullptr);



    init_VentumEngineVariables(); //#0000ff



    GETINITDATA;

    for(int i = 0; i < sizeOfModules;i++)
        Modules[i].init(initData);

    _isInitialized = true;
}


void draw() {
    return;
}

void run() {
    while (!glfwWindowShouldClose(_window)) {
        draw();
        glfwPollEvents();
    }
    return;
}

void init_VentumEngineVariables()
{
    setupRenderArray(&_render);

    return;
}

// void MKTDRAW(VkCommandBuffer cmd)
// {
//     glm::vec3 camPos = { 0.f,-6.f,-10.f };

//     glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
//     glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
//     projection[1][1] *= -1;

//     GPUCameraData camData;
//     camData.proj = projection;
//     camData.view = view;
//     camData.viewproj = projection * view;

//     char boundDefault = 0;

//     void* data;
//     vkMapMemory(_device, get_current_frame()._cameraBufferMemory, 0, sizeof(GPUCameraData), 0, &data);
//     memcpy(data, &camData, sizeof(GPUCameraData));
//     vkUnmapMemory(_device, get_current_frame()._cameraBufferMemory);

//     Mesh* lastMesh = nullptr;
//     Material* lastMaterial = nullptr;

//     for(int iLayer = 0; iLayer < _render.size;iLayer++)
//     {
//         for(int renderObject = 0; renderObject < _render.sizePerArray[iLayer];renderObject++)
//         {
//             if(modesEnabled&MKTDRAWAG)
//                 if(_render.arrayPointers[iLayer][renderObject].type == 0)
//                 {
//                     int fasterID = _render.arrayPointers[iLayer][renderObject].ID;
//                     if(_AGA[fasterID].isVisible)
//                     {
//                         AGPushConstants constants = _AGA[fasterID].AGPC;
//                         MKTAG * object = &_AGA[fasterID].AG;

//                         vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _AGPipeline);

//                         VkDeviceSize offset = 0;

//                         vkCmdPushConstants(cmd, _AGPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &constants);

//                         vkCmdBindVertexBuffers(cmd, 0, 1, &object->_vertexBuffer, &offset);

//                         vkCmdBindIndexBuffer(cmd, _AGA[fasterID].AG.indexBuffer, 0, VK_INDEX_TYPE_UINT16);

//                         vkCmdDrawIndexed(cmd, object->sizeOfIndices, 1, 0, 0, 0);
//                     }
//                 }
//             if(modesEnabled&MKTDRAWPIC)
//                 if(_render.arrayPointers[iLayer][renderObject].type == 1)
//                 {
//                     int fasterID = _render.arrayPointers[iLayer][renderObject].ID;
//                     if(_pictures[fasterID].isVisible)
//                     {
//                         AGPushConstants constants = _pictures[fasterID].PC;
//                         MKTPiC * object = &_defaultPictureRectangle;

//                         VkDeviceSize offset = 0;

//                         vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _DiSPipeline);

//                         vkCmdPushConstants(cmd, _DiSPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &constants);

//                         vkCmdBindVertexBuffers(cmd, 0, 1, &object->_vertexBuffer, &offset);

//                         vkCmdBindIndexBuffer(cmd, _defaultPictureRectangle.indexBuffer , 0, VK_INDEX_TYPE_UINT16);

//                         vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _DiSPipelineLayout, 0, 1, &_currentPicturesDescriptor[fasterID], 0, nullptr);

//                         vkCmdDrawIndexed(cmd, object->sizeOfIndices, 1, 0, 0,0);
//                         // vkCmdDraw(cmd, object->_sizeOfVertices, 1, 0, 0);
//                     }
//                 }
//             if(modesEnabled&MKTDRAW3D)
//                 if(_render.arrayPointers[iLayer][renderObject].type == 2)
//                 {
//                     int fasterID = _render.arrayPointers[iLayer][renderObject].ID;
//                     RenderObject& object = _renderables[fasterID];

//                     if (object.material != lastMaterial) {

//                         vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, object.material->pipeline);
//                         lastMaterial = object.material;
//                         vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, object.material->pipelineLayout, 0, 1, &get_current_frame().globalDescriptor, 0, nullptr);
//                     }


//                     glm::mat4 model = object.transformMatrix;
//                     glm::mat4 mesh_matrix = projection * view * model;

//                     MeshPushConstants constants;
//                     constants.render_matrix = object.transformMatrix;

//                     vkCmdPushConstants(cmd, object.material->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants), &constants);

//                     if (object.mesh != lastMesh) {
//                         VkDeviceSize offset = 0;
//                         vkCmdBindVertexBuffers(cmd, 0, 1, &object.mesh->_vertexBuffer, &offset);
//                         lastMesh = object.mesh;
//                     }
//                     vkCmdDraw(cmd, object.mesh->_sizeOfVertices, 1, 0, 0);
//                 }
//             if(modesEnabled&MKTDRAWTEXT)
//                 if(_render.arrayPointers[iLayer][renderObject].type == 4)
//                 {
//                     int fasterID = _render.arrayPointers[iLayer][renderObject].ID;
//                     if(_TEXT[fasterID].isVisible)
//                     {
//                         TEXTPushConstant constants = _TEXT[fasterID].pushConstant;
//                         MKTAG * object = getFontAG(_TEXT[fasterID]);

//                         vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _AGPipeline);

//                         VkDeviceSize offset = 0;

//                         vkCmdPushConstants(cmd, _fQSPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &constants);

//                         vkCmdBindVertexBuffers(cmd, 0, 1, &object->_vertexBuffer, &offset);

//                         vkCmdBindIndexBuffer(cmd, object->indexBuffer, 0, VK_INDEX_TYPE_UINT16);

//                         vkCmdDrawIndexed(cmd, object->sizeOfIndices, 1, 0, 0, 0);
//                     }
//                 }
            
//         }
//     }
//     if(modesEnabled&MKTDRAWMENU)
//         drawMenu(cmd,Modules, sizeOfModules);
//     return;
// }

// void drawMenu(VkCommandBuffer cmd,GL * menuStuff, size_t sizeOfMenuStuff)
// {
// 	for (int i = 0; i < sizeOfMenuStuff; i++) // make it less than 18 lines per draw
// 	{
//         if(Modules[i].icon.isVisible)
//         {
//             vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _AGPipeline);

//             Modules[i].icon.AGPC.movement.y = CBT+(float)(i*(CBTStatus*ratio)/40);
//             Modules[i].icon.AGPC.movement.x = -0.98+(((CBTStatus-1)/20)/ratio);
//             Modules[i].icon.AGPC.transformation = {(CBTStatus/20)/ratio,0.0,0.0,0.0,0.0,(CBTStatus/40)*ratio,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0};

//             VkDeviceSize offset = 0;
                        
//             vkCmdBindVertexBuffers(cmd, 0, 1, &Modules[i].icon.AG._vertexBuffer, &offset);
            
//             vkCmdBindIndexBuffer(cmd, Modules[i].icon.AG.indexBuffer, 0, VK_INDEX_TYPE_UINT16);

//             vkCmdPushConstants(cmd, _AGPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &Modules[i].icon.AGPC);

//             vkCmdDrawIndexed(cmd, Modules[i].icon.AG.sizeOfIndices, 1, 0, 0, 0);
//         }
//     }

//     for(int i = 0; i < sizeOfMenuStuff;i++) // make it less than 18 lines per draw
//     { 
//         if(Modules[i].icon.isVisible) {

//             vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _AGPipeline);

//             VkDeviceSize offset = 0;
//             vkCmdBindVertexBuffers(cmd, 0, 1, &_AGA[BLACKLINE_ID].AG._vertexBuffer, &offset);

//             vkCmdBindIndexBuffer(cmd, _AGA[BLACKLINE_ID].AG.indexBuffer, 0, VK_INDEX_TYPE_UINT16);

//             AGPushConstants BLC = _AGA[0].AGPC;

//             BLC.movement.y = CBT+(float)(i*(CBTStatus*ratio)/40);
//             BLC.movement.x = -0.98+(((CBTStatus-1)/20)/ratio);
//             BLC.transformation = {(CBTStatus/20)/ratio,0.0,0.0,0.0,0.0,CBTStatus/250,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0};
            
//             vkCmdPushConstants(cmd, _AGPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(AGPushConstants), &BLC);

//             vkCmdDrawIndexed(cmd, _AGA[BLACKLINE_ID].AG.sizeOfIndices, 1, 0, 0, 0);
//         }
//     }
// }