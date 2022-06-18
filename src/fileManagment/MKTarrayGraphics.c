#include "MKTarrayGraphics.h"
#include "../DEBUG.h"
#ifdef __gnu_linux__
#include <cstdlib>
#endif
MKTAG arrayGraphicsReader(const char * filePath)
{
	FILE *MKTFILE = fopen(filePath, "rb" );
	float *list;
	fseek(MKTFILE,0L,SEEK_END);
	long sizeOfFile = ftell(MKTFILE);
	rewind(MKTFILE);
	list = (float*)malloc(sizeOfFile*sizeof(float));
	fread( list,sizeof(float), sizeOfFile, MKTFILE );
	fclose( MKTFILE );
    MKTAG _AG;
    _AG._vertices.resize(((sizeOfFile/sizeof(float))/3)/2);
	for(int i = 0; i < sizeOfFile/sizeof(float);i += 6)
	{
		_AG._vertices[i/6].position = { list[i],list[i+1],list[i+2] };
		_AG._vertices[i/6].color = { list[i+3],list[i+4], list[i+5] };
	}
	return _AG;
}