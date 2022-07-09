#include "./_render.h"

void setupRenderArray(RenderArray * _render)
{
    _render->size = 1;
    _render->sizePerArray = (size_t*)malloc(sizeof(size_t));
    _render->sizePerArray[0] = 1;
    _render->arrayPointers = (DrawStruct**)malloc(sizeof(DrawStruct*));
    _render->arrayPointers[0] = (DrawStruct*)malloc(sizeof(DrawStruct));
    _render->arrayPointers[0][0].ID = 0;
    _render->arrayPointers[0][0].type = 1;
}

void addToRenderArray(RenderArray * _render,char type, int ID, int layer)
{
    if(_render->size < layer)
    {
        _render->sizePerArray = (size_t*)realloc(_render->sizePerArray,sizeof(size_t)*layer+1);
        _render->arrayPointers = (DrawStruct**)realloc(_render->arrayPointers,sizeof(DrawStruct*)*layer+1);
        for(int i = _render->size; i < layer+1;i++)
            _render->sizePerArray = 0;
        _render->size = layer+1;
    }

    _render->sizePerArray[layer]++;
    if(_render->sizePerArray[layer] == 1)
        _render->arrayPointers[layer] = (DrawStruct*)malloc(sizeof(DrawStruct));
    else
        _render->arrayPointers[layer] = (DrawStruct*)realloc(_render->arrayPointers[layer],sizeof(DrawStruct)*_render->sizePerArray[layer]);
    
    _render->arrayPointers[layer][_render->sizePerArray[layer]-1].type = type;
    _render->arrayPointers[layer][_render->sizePerArray[layer]-1].ID = ID;
}