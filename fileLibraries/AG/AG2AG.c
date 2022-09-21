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

    MKTAGdata * outPut = malloc(sizeof(MKTAGdata));

    outPut->sizeOfVertices = *(unsigned long long int*)list;
    outPut->sizeOfIndices = (sizeOfFile-((outPut->sizeOfVertices*24)+8))/sizeof(unsigned int);


    outPut->vertices = malloc(sizeof(AGVertex)*outPut->sizeOfVertices);
    outPut->indices = malloc(sizeof(unsigned int)*outPut->sizeOfIndices);

    for(unsigned long long int i = 0; i < outPut->sizeOfVertices; i++)
        outPut->vertices[i] = ((AGVertex*)&list[8])[i];

    for(unsigned long long int i = 0; i < outPut->sizeOfIndices; i++)
        outPut->indices[i] = ((unsigned int*)(&((AGVertex*)(&list[8]))[outPut->sizeOfVertices-1]))[i];

    output->data = outPut;
    output->sizeOfData = sizeOfFile;
    output->type = 1;
    output->ID = 0;

    return output;
}

unsigned char init(char in)
{
    if(in)
        printf("AG_AG module loaded\n");

    return 0;
}