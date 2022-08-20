#pragma once
#include "types.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct VertexInputDescription {
    VkVertexInputBindingDescription * bindings;
    size_t sizeOfBindings;
    VkVertexInputAttributeDescription * attributes;
    size_t sizeOfAttributes;
    VkPipelineVertexInputStateCreateFlags flags = 0;
};

struct Vertex {

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;

    static VertexInputDescription get_vertex_description();
    static VertexInputDescription getAG_vertex_description();
    static VertexInputDescription getPiC_vertex_description();
    static VertexInputDescription getfQS_vertex_description();
};

struct Mesh {

    Vertex * _vertices;
    size_t _sizeOfVertices;

    VkBuffer _vertexBuffer;
    VkDeviceMemory _vertexBufferMemory;

    bool load_from_obj(const char* filename);
};

typedef struct MKTAGA2 {
    glm::vec3 position;
    glm::vec3 color;
} MKTAGA;

typedef struct MKTAGA3 {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
} MKTAPiC;

typedef struct MKTFQS3 {
    glm::vec3 position;
} MKTfQS;

typedef struct arrayGraphic{ 
    MKTAGA * _vertices;
    size_t _sizeOfVertices;

    VkBuffer _vertexBuffer;
    VkDeviceMemory _vertexBufferMemory;

    unsigned int * indices;
    size_t sizeOfIndices;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
} MKTAG;

typedef struct pictureGraphics{
    MKTAPiC * _vertices;
    size_t _sizeOfVertices;

    VkBuffer _vertexBuffer;
    VkDeviceMemory _vertexBufferMemory;

    unsigned int * indices;
    size_t sizeOfIndices;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
} MKTPiC;