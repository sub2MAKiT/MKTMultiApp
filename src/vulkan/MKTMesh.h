#pragma once
#include "types.h"

struct MKTGraphicsMesh {
    MKTV3 colour;
    MKTV3 position;
    MKTV3 normals;
};

typedef struct MKTGraphicsMesh MKTGraphics3;

struct MKTMesh {
    MKTGraphics3 * _verticies;
    size_t sizeOfArray;
    AllocatedBuffer _vertexBuffer;
};

typedef struct MKTMesh Mesh;
