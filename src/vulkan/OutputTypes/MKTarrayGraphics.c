#include "MKTarrayGraphics.h"
#include "../DEBUG.h"
#ifdef __gnu_linux__
#include <cstdlib>
#endif
MKTAG arrayGraphicsReader(const char * filePath)
{
	FILE *MKTFILE = fopen(filePath, "rb" );
	DEBUG("started loading the AG data");
	float *list;
	fseek(MKTFILE,0L,SEEK_END);
	long sizeOfFile = ftell(MKTFILE);
	int x = sizeOfFile / 28;
	rewind(MKTFILE);
	list = (float*)malloc(sizeOfFile*sizeof(float));
	fread( list,sizeof(float), sizeOfFile, MKTFILE );
	fclose( MKTFILE );
	size_t nVer = *(size_t*)list;
    MKTAG _AG;
    _AG._vertices = (MKTAGA*)malloc(nVer*sizeof(MKTAGA));
	_AG._sizeOfVertices = nVer;
	for(int i = 8; i < nVer*6+8;i += 6)
	{
		_AG._vertices[(i-8)/6].position = { list[i-6],list[i-5],list[i-4] };
		_AG._vertices[(i-8)/6].color = { list[i-3],list[i-2], list[i-1] };
	}
	_AG.sizeOfIndices = (sizeOfFile-8-24*nVer)/4;
	_AG.indices = (unsigned int*)malloc(_AG.sizeOfIndices*sizeof(int));
	for(int i = 0; i < (sizeOfFile-8-24*nVer)/4;i++)
		_AG.indices[i] = *(unsigned int*)&list[i+2+nVer*6];

	DEBUG("loaded the AG data");
	return _AG;
}