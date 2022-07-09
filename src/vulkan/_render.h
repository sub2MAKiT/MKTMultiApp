#include <stdlib.h>

typedef struct MainRenderStruct {
    int ID;
    char type; // 0 - AG, 1 - picture, 2 - object
} DrawStruct;

typedef struct MainRenderArray {
    DrawStruct ** arrayPointers;
    size_t * sizePerArray;
    size_t size;
} RenderArray;

void setupRenderArray(RenderArray * render);
void addToRenderArray(RenderArray * _render,char type, int ID, int layer);