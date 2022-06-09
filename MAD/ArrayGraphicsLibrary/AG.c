#include <stdio.h>
#include <stdlib.h>

extern char MKTcompStr(char * charArray, const char * string);
void generateShape();

void entry(void * ptr)
{
    FILE *MKTFILE;
    MKTFILE = fopen( "./lib/libraryList", "rb" );
    char mode;
    if( MKTFILE != NULL )
    {
        fread( &mode,1, 1, MKTFILE );
    } else {
        printf("\nMode not detected, AG might not work correctly");
    }

    if(mode)
    {
        char * charArray = (char*)ptr;
        if(MKTcompStr(charArray, "genShape"))
            generateShape();
    }

    return;
}

void generateShape()
{
    printf("\nNumber of verteces: ");
    int ver;
    scanf("%d",&ver);
    float * vertX = (float*)malloc(sizeof(float)*ver);
    float * vertY = (float*)malloc(sizeof(float)*ver);
    float * vertZ = (float*)malloc(sizeof(float)*ver);
    float * colour = (float*)malloc(sizeof(float)*3);
    for(int i = 0; i < ver; i++)
    {
        printf("\nPosition X of the vertex number %d: ",i);
        scanf("%f",vertX+i);
        printf("\nPosition Y of the vertex number %d: ",i);
        scanf("%f",vertY+i);
        printf("\nDepth of the vertex number %d: ",i);
        scanf("%f",vertZ+i);
    }

    printf("\nRed value: ");
    scanf("%f",colour);
    printf("\nGreen value: ");
    scanf("%f",colour+1);
    printf("\nBlue value: ");
    scanf("%f",colour+2);

    FILE *MKTFILE;
    MKTFILE = fopen( "./temp/AG/generated.MKTAG", "wb" );
    if( MKTFILE != NULL )
    {
        for(int i = 0; i < ver-2;i++)
        {
            for(int a = 0; a < 3; a++)
            {
                printf("\nwriting ver[%d]",a);
                fwrite(((float*)vertX+a),sizeof(float),1,MKTFILE);
                fwrite(((float*)vertY+a),sizeof(float),1,MKTFILE);
                fwrite(((float*)vertZ+a),sizeof(float),1,MKTFILE);
                fwrite(colour,sizeof(float),3,MKTFILE);
            }

            for(int a = 1; a < ver - i - 1; a++)
            {
                vertX[a] = vertX[a+1];
                vertY[a] = vertY[a+1];
                vertZ[a] = vertZ[a+1];
            }

            vertX = realloc(vertX,sizeof(float)*ver-(i+1));
            vertY = realloc(vertY,sizeof(float)*ver-(i+1));
            vertZ = realloc(vertZ,sizeof(float)*ver-(i+1));
        }
    } else {
        printf("\n Couldn't open the file");
    }

    fclose(MKTFILE);

    return;
}