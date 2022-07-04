#include "MKTPicture.h"

MKTPic MKTPicReader(const char * FP)
{
    MKTPic picToBeRead;
    FILE *MKTFILE = fopen(FP, "rb" );
	char *list;
	fseek(MKTFILE,0L,SEEK_END);
	size_t sizeOfFile = ftell(MKTFILE);
	rewind(MKTFILE);
	list = (char*)malloc(sizeOfFile);
	fread( list,1, sizeOfFile, MKTFILE );

    picToBeRead.width = *(size_t*)list;
    picToBeRead.pixels = (MKTPix*)malloc(sizeOfFile-sizeof(size_t));

    for(int i = 0; i < sizeOfFile-sizeof(size_t);i++)
    {
        if(i%4 == 0)
            picToBeRead.pixels[(i-(i%4))/4].R = list[i+sizeof(size_t)];
        if(i%4 == 1)
            picToBeRead.pixels[(i-(i%4))/4].G = list[i+sizeof(size_t)];
        if(i%4 == 2)
            picToBeRead.pixels[(i-(i%4))/4].B = list[i+sizeof(size_t)];
        if(i%4 == 3)
            picToBeRead.pixels[(i-(i%4))/4].A = list[i+sizeof(size_t)];
    }
	
    fclose( MKTFILE );

    picToBeRead.height = (sizeOfFile-sizeof(size_t))/sizeof(MKTPix)/picToBeRead.width;
    // if(DEBUG("loadImage"))
        // for(int i = 0; i < picToBeRead.height * picToBeRead.width;i++)
            // printf("\ntesting the picture: %d %d %d %d",picToBeRead.pixels[i].R,picToBeRead.pixels[i].G,picToBeRead.pixels[i].B,picToBeRead.pixels[i].A);

    return picToBeRead;
}