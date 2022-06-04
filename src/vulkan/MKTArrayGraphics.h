#pragma once
#include "types.h"

struct MKTGraphicsArray {
    MKTV3 colour;
    MKTV3 position;
};

typedef struct MKTGraphicsArray MKTGraphics2;

struct MKTarrayGraphic {
    MKTGraphics2 * _verticies;
    size_t sizeOfArray;
    AllocatedBuffer _vertexBuffer;
};

typedef struct MKTarrayGraphic arrayGraphic;