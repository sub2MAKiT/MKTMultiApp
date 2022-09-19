#include <stdlib.h>
#include <stdio.h>
#include <MKTDLL/dllDefines.h>
#include <vulkan/structs.h>

void * data;
unsigned long long int sizeOfData;
int ID;
char type;

MKTInfo * load(char * FP)
{
    MKTInfo * output = malloc(sizeof(MKTInfo));

    FILE * MKTFILE = fopen(FP,"rb");

    void *list;
    fseek(MKTFILE, 0L, SEEK_END);
    unsigned long sizeOfFile = ftell(MKTFILE);
    rewind(MKTFILE);
    list = malloc(sizeOfFile);
    sizeOfData = fread( list,1, sizeOfFile, MKTFILE );
    fclose( MKTFILE );

    output->data = list;
    output->sizeOfData = sizeOfFile;
    output->type = 1;

    return output;
}

char * init(char in)
{
    char * a = malloc(1);
    
    if(in)
        printf("RAW_PiC module loaded\n");

    return a;
}