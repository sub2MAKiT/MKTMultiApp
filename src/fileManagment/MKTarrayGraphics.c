#include "MKTarrayGraphics.h"
#include "../DEBUG.h"
#ifdef __gnu_linux__
#include <cstdlib>
#endif
MKTAG arrayGraphicsReader(const char * filePath)
{
	FILE *MKTFILE = fopen(filePath, "rb" );
	printf("\n\nso: %x %s\n\n",MKTFILE,filePath);
	float *list;
                printf("\ntest0 1\n");
	fseek(MKTFILE,0L,SEEK_END);
                printf("\ntest0 1,5\n");
	long sizeOfFile = ftell(MKTFILE);
                printf("\ntest0 2\n");
                printf("\ntest0 2\n");
	rewind(MKTFILE);
	list = (float*)malloc(sizeOfFile*sizeof(float));
                printf("\ntest0 3\n");
	fread( list,sizeof(float), sizeOfFile, MKTFILE );
	fclose( MKTFILE );
                printf("\ntest0 4\n");
    MKTAG _AG;
    _AG._vertices.resize(((sizeOfFile/sizeof(float))/3)/2);
                printf("\ntest0 5\n");
	for(int i = 0; i < sizeOfFile/sizeof(float);i += 6)
	{
                printf("\ntest1 %d",i);
		_AG._vertices[i/6].position = { list[i],list[i+1],list[i+2] };
                printf("\ntest2 %d",i);
		_AG._vertices[i/6].color = { list[i+3],list[i+4], list[i+5] };
                printf("\ntest3 %d",i);
	}
	return _AG;
}