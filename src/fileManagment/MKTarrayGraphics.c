#include "MKTarrayGraphics.h"

MKTAG arrayGraphicsReader(char * filePath)
{
    MKTAG _AG;
    _AG._vertices.resize(6);
	_AG._vertices[0].position = { 1.f,0.f,0.5f };
	_AG._vertices[1].position = { -1.f,0.f,0.5f};
	_AG._vertices[2].position = { 0.f,-1.f, 0.5f};

	_AG._vertices[0].color = { 1.f,0.f, 0.0f };
	_AG._vertices[1].color = { 0.f,1.f, 0.0f }; 
	_AG._vertices[2].color = { 0.f,0.f, 1.0f };

	_AG._vertices[3].position = { 1.f,0.f,0.5f};
	_AG._vertices[4].position = { -1.f,0.f,0.5f};
	_AG._vertices[5].position = { 0.f,1.f, 0.5f};

	_AG._vertices[3].color = { 1.f,0.f, 0.0f };
	_AG._vertices[4].color = { 0.f,1.f, 0.0f }; 
	_AG._vertices[5].color = { 0.f,0.f, 0.0f };
    return _AG;
}