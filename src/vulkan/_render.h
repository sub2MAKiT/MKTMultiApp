#include <stdlib.h>

typedef struct MainRenderStruct {
    int ID;
    char type; // 0 - AG, 1 - picture, 2 - object, 4 - text
} DrawStruct;

typedef struct MainRenderArray {
    DrawStruct ** arrayPointers;
    size_t * sizePerArray;
    size_t size;
} RenderArray;

void setupRenderArray(RenderArray * render);
void addToRenderArray(RenderArray * _render,char type, int ID, int layer);
void reallocateRenderArray(RenderArray * _render,int inLayer, int outLayer,int ID, char type);
int findRenderObject(RenderArray * _render,int layer,int ID, char type);
void removeRenderObject(RenderArray * _render,int layer,int ID, char type);
void freeRenderArray(RenderArray * render);