#include <stdio.h>
#include <stdlib.h>

typedef struct paletteDataStruct {
int colour;
unsigned long long int n;
} paletteDataS;


static unsigned long long int colourDifference(int pixelData, int expected)
{
    unsigned long long int difference = 0;

    char inputR = *((char*)&pixelData);
    char inputG = *((char*)&pixelData+1);
    char inputB = *((char*)&pixelData+2);
    char inputA = *((char*)&pixelData+3);

    char expectedR = *((char*)&expected);
    char expectedG = *((char*)&expected+1);
    char expectedB = *((char*)&expected+2);
    char expectedA = *((char*)&expected+3);

    difference += inputR-expectedR>0?inputR-expectedR:expectedR-inputR;
    difference += inputG-expectedG>0?inputG-expectedG:expectedG-inputG;
    difference += inputB-expectedB>0?inputB-expectedB:expectedB-inputB;
    difference += inputA-expectedA>0?inputA-expectedA:expectedA-inputA;

    // difference = *((char*)&pixelData)-*((char*)&expected)>0?*((char*)&pixelData)-*((char*)&expected):*((char*)&expected)-*((char*)&pixelData) +
    // *((char*)&pixelData+1)-*((char*)&expected+1)>0?*((char*)&pixelData+1)-*((char*)&expected+1):*((char*)&expected+1)-*((char*)&pixelData+1) +
    // *((char*)&pixelData+2)-*((char*)&expected+2)>0?*((char*)&pixelData+2)-*((char*)&expected+2):*((char*)&expected+2)-*((char*)&pixelData+2) +
    // *((char*)&pixelData+3)-*((char*)&expected+3)>0?*((char*)&pixelData+3)-*((char*)&expected+3):*((char*)&expected+3)-*((char*)&pixelData+3);

    // printf("R: %d G: %d B: %d A: %d\nR: %d G: %d B: %d A: %d\n%ld\n",inputR,inputG,inputB,inputA,expectedR,expectedG,expectedB,expectedA,difference);
    
    return difference;
}


static unsigned long long int findBestN(int pixelData, paletteDataS * comp,unsigned long long int sizeOfFilteredPalette)
{
    unsigned long long int smallestCD = colourDifference(comp[0].colour,pixelData);
    unsigned long long int returnID = 0;

    for(unsigned long long int i = 0; i < sizeOfFilteredPalette; i++)
    {
        if(smallestCD > colourDifference(comp[i].colour,pixelData))
        {
            smallestCD = colourDifference(comp[i].colour,pixelData);
            returnID = i;
            if(smallestCD == 0)
                return returnID;
        }
    }

    return returnID;
}

int main(int argc, char ** argv)
{
    if(argc != 4)
        printf("Usage:\n./raw2PiC.exe {input} {output} {compression index}\n");
    else
    {
        FILE * INFILE = fopen(argv[1],"rb");
        FILE * OUTFILE = fopen(argv[2],"wb");

        unsigned char compressionIndex = argv[3][0]-'0'+argv[3][1]!=0?(argv[3][1]-'0')*10:0;

        unsigned char interpolate = compressionIndex&8;

        compressionIndex = compressionIndex&7;

        printf("compression: %x %x\n",interpolate,compressionIndex);

        char *input;// #ff0000
        fseek(INFILE, 0L, SEEK_END);
        unsigned long long int sizeOfFile = ftell(INFILE);
        rewind(INFILE);
        input = (char*)malloc(sizeOfFile);
        sizeOfFile = fread( input,1, sizeOfFile, INFILE );

        unsigned long long int width = *(unsigned long long int*)input;
        unsigned long long int height = *(unsigned long long int*)&input[8];

        int * pixelData = (int*)&input[16];
        paletteDataS * palette = malloc(sizeof(paletteDataS)); // #ff0000

        unsigned long long int biggestN = 0;

        fwrite(&compressionIndex,sizeof(unsigned char),1,OUTFILE);
        fwrite(&width,sizeof(unsigned long long int),1,OUTFILE);
        fwrite(&height,sizeof(unsigned long long int),1,OUTFILE);

        #define CPS unsigned char
        #define MAXCPS 255

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
            unsigned long long int sizeOfFileteredPalette = 0;

            // here, we will have to sort the paletteData, and given that I'm doing this a few weeks/months before my sorting video, good luck to me.
            // algorythm I picked is a bit unsophisticated, but given that I have a fixed size to fill, it is the best* approach

//* the best my clown brain could've came up with

            for(unsigned long long int i = 0; i < MAXCPS&&i<sizeOfPalette;i++)
            {
                unsigned long long int highest = palette[i].n;
                unsigned long long int currentIndex = 0;
                for(unsigned long long a = 0; a < sizeOfPalette; a++)
                {
                    if(palette[a].n > highest)
                    {
                        char isAlreadyFiltered = 0;
                        for(int b = 0; b < sizeOfFileteredPalette;b++)
                        {
                            if(filteredPalette[b].colour == palette[a].colour)
                            isAlreadyFiltered = 1;
                        }

                        if(!isAlreadyFiltered)
                        {
                            highest = palette[a].n;
                            currentIndex = a;
                        }
                    }
                }
                sizeOfFileteredPalette++;
                filteredPalette = realloc(filteredPalette,sizeOfFileteredPalette*sizeof(paletteDataS));
                filteredPalette[i] = palette[currentIndex];
            }

            // for(unsigned int i = 0; i < sizeOfFileteredPalette;i++)
                // printf("%d.%d %d\n",i,filteredPalette[i].n,filteredPalette[i].colour);

            // for(unsigned int i = 0; i < sizeOfFileteredPalette;i++)
                // printf("%d.%x %x %d\n",i,filteredPalette[i].colour,palette[i].colour,colourDifference(filteredPalette[i].colour,palette[i].colour));

            printf("sizeOfFilteredPalette: %d\n",sizeOfFileteredPalette);

            free(palette);

            for(unsigned long long int i = 0; i < sizeOfFileteredPalette; i++)
                fwrite(&filteredPalette[i].colour,sizeof(int),1,OUTFILE);

            for(unsigned long long int i = 0; i < width*height; i++)
            {
                CPS output = findBestN(pixelData[i],filteredPalette,sizeOfFileteredPalette);
                // printf("%d.test %d %d %d %d\n",i,output,filteredPalette[output].colour,pixelData[i],colourDifference(filteredPalette[output].colour,pixelData[i]));
                fwrite(&output,sizeof(CPS),1,OUTFILE);
            }

        }

        printf("output: %d %d %d %d\n",compressionIndex,width,height, biggestN);

        fclose(INFILE);
        fclose(OUTFILE);
    }
    return 0;
}