#include "../FILEDLL.m"

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

    MKTbmdata * outPut = malloc(sizeof(MKTbmdata));

    outPut->w = *(unsigned long long int*)&list[0];
    outPut->h = *(unsigned long long int*)&list[8];
    outPut->pix = malloc(outPut->w*outPut->h);

    for(unsigned long long int i = 0; i < outPut->w*outPut->h;i++)
        *((char*)(outPut->pix)+i) = list[i+16];
        
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
    output->type = 5;

    free(list);

    return output;
}

unsigned char init(char in)
{
    if(in)
        printf("BM_BM module loaded\n");

    return 1;
}