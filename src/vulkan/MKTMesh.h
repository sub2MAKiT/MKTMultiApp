#pragma once
#include "types.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct VertexInputDescription {
    std::vector<VkVertexInputBindingDescription> bindings;
    std::vector<VkVertexInputAttributeDescription> attributes;

    VkPipelineVertexInputStateCreateFlags flags = 0;
};

struct Vertex {

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;

    static VertexInputDescription get_vertex_description();
    static VertexInputDescription getAG_vertex_description();
    static VertexInputDescription getPiC_vertex_description();
};

struct Mesh {

    std::vector<Vertex> _vertices;

    AllocatedBuffer _vertexBuffer;

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

typedef struct arrayGraphic{ 
    std::vector<MKTAGA> _vertices;
    AllocatedBuffer _vertexBuffer;
} MKTAG;

typedef struct pictureGraphics{
    std::vector<MKTAPiC> _vertices;
    AllocatedBuffer _vertexBuffer;
} MKTPiC;