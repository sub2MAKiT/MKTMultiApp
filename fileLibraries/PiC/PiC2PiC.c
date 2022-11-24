#include "../FILEDLL.m"

#define CPSLOGIC(CPS,MAXCPS)         int * palette = (int*)&list[25];\
        CPS * indicesData = (CPS*)&list[25+sizeOfFilteredPalette*sizeof(CPS)*(MAXCPS==254?4:2)];/*#ff0000*/\ 
        for(unsigned long long int i = 0, j = 0; i < outPut->w*outPut->h;i++, j++)\
            if(indicesData[j] != MAXCPS)\
                ((int*)outPut->pix)[i] = palette[indicesData[j]];\
            else\
            {\
                for(unsigned long long int a = i; i-a < indicesData[j+1]; i++)\
                    ((int*)outPut->pix)[i] = palette[indicesData[j+2]];\
                j+=2;\
            }


MKTInfo * load(char * FP)
{
    MKTInfo * output = malloc(sizeof(MKTInfo));

    FILE * MKTFILE = fopen(FP,"rb");

    char *list;
    fseek(MKTFILE, 0L, SEEK_END);
    unsigned long sizeOfFile = ftell(MKTFILE);
    rewind(MKTFILE);
    list = malloc(sizeOfFile);
    sizeOfFile = fread( list,1, sizeOfFile, MKTFILE );
    fclose( MKTFILE );

    MKTPiCdata * outPut = malloc(sizeof(MKTPiCdata));

    unsigned char compressionIndex = list[0];
    unsigned long long int sizeOfFilteredPalette = *(unsigned long long int*)&list[17];

    // printf("palette size: %d\n",sizeOfFilteredPalette);
    outPut->w = *(unsigned long long int*)&list[1];
    outPut->h = *(unsigned long long int*)&list[9];
    outPut->pix = malloc(outPut->w*outPut->h*sizeof(MKTrgbaP));


    if(compressionIndex == 4)
    {
        CPSLOGIC(unsigned char,254)
    } else if(compressionIndex == 3)
    {
        CPSLOGIC(unsigned short,65534)
    } else if(compressionIndex == 2)
    {
        CPSLOGIC(unsigned int,4294967295 )
    } else if(compressionIndex == 1)
    {
        // CPSLOGIC(unsigned long long int,18446744073709551615)
    } else if(compressionIndex == 0)
    {
        CPSLOGIC(unsigned char,255)
    }


    outPut->vertices = malloc(sizeof(PiCVertex)*4);
    outPut->vertices[0] = (PiCVertex){{1.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}};
    outPut->vertices[1] = (PiCVertex){{1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}};
    outPut->vertices[2] = (PiCVertex){{-1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};
    outPut->vertices[3] = (PiCVertex){{-1.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}};

    outPut->indices = malloc(sizeof(unsigned int)*6);
    outPut->sizeOfIndices = 6;
    outPut->sizeOfVertices = 4;

    outPut->indices[0] = 0;
    outPut->indices[1] = 1;
    outPut->indices[2] = 2;
    outPut->indices[3] = 0;
    outPut->indices[4] = 2;
    outPut->indices[5] = 3;

    output->data = outPut;
    output->sizeOfData = sizeOfFile;
    output->type = 2;

    free(list);

    return output;
}

unsigned char init(char in)
{
    if(in)
        printf("RAW_PiC module loaded\n");

    return 1;
}


/* 
file architecture:
width      | 8b
height     | 8b
numOfVer   | 8b
Palette    | x
pixelData  | x
vertices   | x
indices    | x
*/

/*
PixelData:

r,g,b,a
   \/
255 compression
*/