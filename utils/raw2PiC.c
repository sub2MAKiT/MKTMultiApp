#include <stdio.h>
#include <stdlib.h>



typedef struct paletteDataStruct {
long long int colour;
unsigned long long int n;
} paletteDataS;

static unsigned long long int findBestN(long long int pixelData, paletteDataS * comp)
{

}

int main(int argc, char ** argv)
{
    if(argc != 4)
        printf("Usage:\n./raw2PiC.exe {input} {output} {compression index}\n");
    else
    {
        FILE * INFILE = fopen(argv[1],"rb");
        FILE * OUTFILE = fopen(argv[2],"wb");

        unsigned char compressionIndex = argv[3][0]-'0';

        char *input;// #ff0000
        fseek(INFILE, 0L, SEEK_END);
        unsigned long long int sizeOfFile = ftell(INFILE);
        rewind(INFILE);
        input = (char*)malloc(sizeOfFile);
        sizeOfFile = fread( input,1, sizeOfFile, INFILE );

        unsigned long long int width = *(unsigned long long int*)input;
        unsigned long long int height = *(unsigned long long int*)&input[8];

        long long int * pixelData = (long long int*)&input[16];
        paletteDataS * palette = malloc(sizeof(paletteDataS)); // #ff0000

        unsigned long long int biggestN = 0;

        #define CPS char

        if(compressionIndex == 4) // 0 - long long, 1 - long, 2 - int, 3 - short, 4 - char
        {
            
            CPS * outputPixelData = malloc(sizeof(CPS));// #ff0000
            unsigned long long int sizeOfOutputPixelData = 0;


            unsigned long long int sizeOfPalette = 0;
            for(int i = 0; i < width*height; i++)
            {
                char isInPalette = 0;
                for(unsigned int a = 0; a < sizeOfPalette;a++)
                {
                    if(pixelData[i] == palette[a].colour)
                    {
                        isInPalette = 1;
                        palette[a].n++;

                        if(palette[a].n > biggestN)
                            biggestN = palette[a].n;
                        
                        // fwrite(&a,sizeof(unsigned int),1,OUTFILE);
                        break;
                    }
                }
                if(!isInPalette)
                {
                    sizeOfPalette++;
                    palette = realloc(palette,sizeOfPalette*sizeof(paletteDataS));
                    palette[sizeOfPalette-1].colour = pixelData[i];
                    palette[sizeOfPalette-1].n = 0;
                }
            }


            paletteDataS * filteredPalette = malloc(sizeof(paletteDataS)); // #ff0000

            // here, we will have to sort the paletteData, and given that I'm doing this a few weeks/months before my sorting video, good luck to me.

            for(unsigned long long int i = 0; i < sizeof(CPS)&&i<sizeOfPalette;0)
            {

            }

            free(palette);

            for(unsigned long long int i = 0; i < width*height; i++)
            {
                CPS output = findBestN(pixelData[i],palette);
                // fwrite(&output,sizeof(CPS),1,OUTFILE);
            }

        }

        printf("output: %d %d %d %d\n",compressionIndex,width,height, biggestN);

        fclose(INFILE);
        fclose(OUTFILE);
    }
    return 0;
}