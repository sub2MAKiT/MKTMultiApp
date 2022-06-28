#include <stdio.h>
#include <stdlib.h>

typedef struct MKTRGBAPixel {
unsigned char R;
unsigned char G;
unsigned char B;
unsigned char A;   
} MKTPix;

typedef struct MKTRGBApicture {
MKTPix * pixels;
size_t width;
size_t height;
} MKTPic;

MKTPic MKTPicReader(const char * FP);