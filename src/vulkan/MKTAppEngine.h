#pragma once
#include <stdio.h>
#include <stdlib.h>

// MKT Vulkan Includes
#include <defines.h>
#include <utils/_render.h>
#include <types.h>
#include <utils/MKTbuffer.h>
#include <boot/boot.h>
#include <outputTypes/MKTarrayGraphics.h>

// MKTMultiApp
#include "../libraryHeader.h"
#include "../utils.h"

// OS specific (to remove with MKT-base_devel)
#ifdef _WIN32
#include <windows.h>
#elif __gnu_linux__
#include <dlfcn.h>
#endif

// GLFW
#include <GLFW/glfw3native.h>
#include <GLFW/glfw3.h>

// Output Types
// #include <OutputTypes/MKTMesh.h>
#include <OutputTypes/OTDataStructs.h>

// constexpr unsigned int FRAME_OVERLAP = 2;

// typedef struct uintVertex {
//     unsigned int * array;
//     size_t size;
// } UINTV;

typedef struct MKTEXT {
    TEXTPushConstant pushConstant;
    char * text;
    size_t sizeOfText;
    unsigned int font;
    char isVisible;
} MKTTEXT;

// typedef struct RenderObject {
// 	Mesh* mesh;
//     VkPipeline * pPipeline;
// 	VkPipelineLayout * pPipelineLayout;
// 	glm::mat4 transformMatrix;
// } MKT3D;

typedef struct defaultPicStruct {
    MKTVKPiC Pic;
    AGPushConstants AGPC;
    char isVisible;
} MKTPiC;

typedef struct finalAGToRender{
    MKTVKAG AG;
    AGPushConstants AGPC;
    char isVisible;
} MKTAG;

typedef struct FrameDataStruct {
	VkSemaphore _presentSemaphore, _renderSemaphore;
	VkFence _renderFence;	

	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;

    VkBuffer _cameraBuffer;
    VkDeviceMemory _cameraBufferMemory;

    VkDescriptorSet globalDescriptor;
} FrameData;

typedef struct UploadContextStruct {
	VkFence _uploadFence;
	VkCommandPool _commandPool;
	VkCommandBuffer _commandBuffer;
} UploadContext;

typedef struct LoadedModule {
    MKTAG icon;
    void (*init)(initi a);
    void (*run)(funi a);
    void (*cleanUp)(cleanUpi a);
} GL; // goodLuck

// MKTMultiApp Variables
// extern GL * Modules;
// extern size_t sizeOfModules;


// Global variables
#ifdef _MKTENGINEINCLUDEGUARD_

#ifdef MKT_DEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


void init();
void cleanup();
void draw();
void run();

VkPipelineLayout _trianglePipelineLayout;
VkPipelineLayout _AGPipelineLayout;
VkPipelineLayout _DiSPipelineLayout;
VkPipelineLayout _meshPipelineLayout;
VkPipelineLayout _fQSPipelineLayout;

VkPipeline _pinkTrianglePipeline;
VkPipeline _trianglePipeline;
VkPipeline _AGPipeline;
VkPipeline _DiSPipeline;
VkPipeline _meshPipeline;
VkPipeline _fQSPipeline;

#else

#endif