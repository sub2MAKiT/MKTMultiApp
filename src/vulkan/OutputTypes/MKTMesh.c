#include "MKTMesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tol/tiny_obj_loader.h>
#include <iostream>

VertexInputDescription Vertex::get_vertex_description()
{
    VertexInputDescription description;

    VkVertexInputBindingDescription mainBinding;
    mainBinding.binding = 0;
    mainBinding.stride = sizeof(Vertex);
    mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    description.bindings = (VkVertexInputBindingDescription*)malloc(sizeof(VkVertexInputBindingDescription));
    *description.bindings = mainBinding;

    VkVertexInputAttributeDescription positionAttribute = {};
    positionAttribute.binding = 0;
    positionAttribute.location = 0;
    positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    positionAttribute.offset = offsetof(Vertex, position);

    VkVertexInputAttributeDescription normalAttribute = {};
    normalAttribute.binding = 0;
    normalAttribute.location = 1;
    normalAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    normalAttribute.offset = offsetof(Vertex, normal);

    VkVertexInputAttributeDescription colorAttribute = {};
    colorAttribute.binding = 0;
    colorAttribute.location = 2;
    colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    colorAttribute.offset = offsetof(Vertex, color);

    description.attributes = (VkVertexInputAttributeDescription*)malloc(sizeof(VkVertexInputAttributeDescription)*3);
    
    description.attributes[0] = positionAttribute;
    description.attributes[1] = normalAttribute;
    description.attributes[2] = colorAttribute;
    return description;
}

VertexInputDescription Vertex::getAG_vertex_description()
{
    VertexInputDescription description;

    VkVertexInputBindingDescription mainBinding;
    mainBinding.binding = 0;
    mainBinding.stride = sizeof(MKTAGA2);
    mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    description.bindings = (VkVertexInputBindingDescription*)malloc(sizeof(VkVertexInputBindingDescription));
    *description.bindings = mainBinding;

    VkVertexInputAttributeDescription positionAttribute = {};
    positionAttribute.binding = 0;
    positionAttribute.location = 0;
    positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    positionAttribute.offset = offsetof(MKTAGA2, position);

    VkVertexInputAttributeDescription colorAttribute = {};
    colorAttribute.binding = 0;
    colorAttribute.location = 1;
    colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    colorAttribute.offset = offsetof(MKTAGA2, color);
    
    description.attributes = (VkVertexInputAttributeDescription*)malloc(sizeof(VkVertexInputAttributeDescription)*2);
    
    description.attributes[0] = positionAttribute;
    description.attributes[1] = colorAttribute;
    return description;
}

VertexInputDescription Vertex::getPiC_vertex_description()
{
    VertexInputDescription description;

    VkVertexInputBindingDescription mainBinding;
    mainBinding.binding = 0;
    mainBinding.stride = sizeof(MKTAPiC);
    mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    description.bindings = (VkVertexInputBindingDescription*)malloc(sizeof(VkVertexInputBindingDescription));
    *description.bindings = mainBinding;

    VkVertexInputAttributeDescription positionAttribute = {};
    positionAttribute.binding = 0;
    positionAttribute.location = 0;
    positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    positionAttribute.offset = offsetof(MKTAPiC, position);

    VkVertexInputAttributeDescription colorAttribute = {};
    colorAttribute.binding = 0;
    colorAttribute.location = 1;
    colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    colorAttribute.offset = offsetof(MKTAPiC, color);

    VkVertexInputAttributeDescription texCoorAttribute = {};
    texCoorAttribute.binding = 0;
    texCoorAttribute.location = 2;
    texCoorAttribute.format = VK_FORMAT_R32G32_SFLOAT;
    texCoorAttribute.offset = offsetof(MKTAPiC, texCoord);
    
    description.attributes = (VkVertexInputAttributeDescription*)malloc(sizeof(VkVertexInputAttributeDescription)*3);
    
    description.attributes[0] = positionAttribute;
    description.attributes[1] = colorAttribute;
    description.attributes[2] = texCoorAttribute;
    return description;
}

VertexInputDescription Vertex::getfQS_vertex_description()
{
    VertexInputDescription description;

    VkVertexInputBindingDescription mainBinding;
    mainBinding.binding = 0;
    mainBinding.stride = sizeof(MKTfQS);
    mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    description.bindings = (VkVertexInputBindingDescription*)malloc(sizeof(VkVertexInputBindingDescription));
    *description.bindings = mainBinding;

    VkVertexInputAttributeDescription positionAttribute = {};
    positionAttribute.binding = 0;
    positionAttribute.location = 0;
    positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    positionAttribute.offset = offsetof(MKTfQS, position);

    description.attributes = (VkVertexInputAttributeDescription*)malloc(sizeof(VkVertexInputAttributeDescription));
    
    *description.attributes = positionAttribute;

    return description;
}

bool Mesh::load_from_obj(const char* filename)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    _sizeOfVertices = 0;
    _vertices = (Vertex*)malloc(sizeof(Vertex));

	std::string warn;
	std::string err;

	tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename, nullptr);
	if (!warn.empty()) {
		std::cout << "WARN: " << warn << std::endl;
	}
	if (!err.empty()) {
		std::cerr << err << std::endl;
		return false;
	}

    for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {

			int fv = 3;

			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

            	tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

				Vertex new_vert;
				new_vert.position.x = vx;
				new_vert.position.y = vy;
				new_vert.position.z = vz;

				new_vert.normal.x = nx;
				new_vert.normal.y = ny;
                new_vert.normal.z = nz;

                new_vert.color = new_vert.normal;

                _sizeOfVertices++;
				_vertices = (Vertex*)realloc(_vertices,sizeof(Vertex)*_sizeOfVertices);
                _vertices[_sizeOfVertices-1] = new_vert;

			}
			index_offset += fv;
		}
	}

    return true;
}