#include <stdio.h>
#include <stdlib.h>

typedef struct RGBA {
unsigned char R;
unsigned char G;
unsigned char B;
unsigned char A;
} colour;

int main()
{
    FILE * MKTFILE = fopen("./build/graphics/MKTPhotos/logo.MKTP","wb");
    size_t * size = (size_t*)malloc(sizeof(size_t));
    *size = 2;
    fwrite(size,1,sizeof(size_t),MKTFILE);
    colour C;
    C.A = 255;
    C.R = 128;
    C.G = 128;
    C.B = 0;
    fwrite(&C,1,sizeof(colour),MKTFILE);
    C.A = 255;
    C.R = 0;
    C.G = 0;
    C.B = 255;
    fwrite(&C,1,sizeof(colour),MKTFILE);
    C.A = 255;
    C.R = 0;
    C.G = 255;
    C.B = 0;
    fwrite(&C,1,sizeof(colour),MKTFILE);
    C.A = 255;
    C.R = 255;
    C.G = 0;
    C.B = 0;
    fwrite(&C,1,sizeof(colour),MKTFILE);
    fclose(MKTFILE);




    MKTFILE = fopen("./build/graphics/MKTPhotos/wideColoursTest.MKTP","wb");
    *size = 10;
    fwrite(size,1,sizeof(size_t),MKTFILE);

    C.A = 255;
    C.R = 128;
    C.G = 128;
    C.B = 0;
    for(int i = 0; i < 50;i++)
        fwrite(&C,1,sizeof(colour),MKTFILE);
    C.A = 255;
    C.R = 0;
    C.G = 0;
    C.B = 255;
    for(int i = 0; i < 50;i++)
        fwrite(&C,1,sizeof(colour),MKTFILE);
    C.A = 255;
    C.R = 0;
    C.G = 255;
    C.B = 0;
    for(int i = 0; i < 50;i++)
        fwrite(&C,1,sizeof(colour),MKTFILE);
    C.A = 255;
    C.R = 255;
    C.G = 0;
    C.B = 0;
    for(int i = 0; i < 50;i++)
        fwrite(&C,1,sizeof(colour),MKTFILE);
    fclose(MKTFILE);
    return 0;
}