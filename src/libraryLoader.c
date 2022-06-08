#define MKT_DLL_LOADING
#include "libraryLoader.h"

long Shmodules = 0;
FUNHANDLE * hmodules = NULL;

#ifdef _WIN32

char * windowsDLLLoading()
{
    char librariesPath[15] = "./lib/windows/";
    hmodules = (HMODULE*)malloc(sizeof(HMODULE));
    FILE *MKTFILE;
    char * notLoaded = (char*)malloc(1);
    size_t sizeOfNotLoaded = 0;
    MKTFILE = fopen( "./lib/libraryList", "rb" );
    if( MKTFILE != NULL )
    {
        char *charArray;
        fseek(MKTFILE, 0L, SEEK_END);
        long sizeOfFile = ftell(MKTFILE);
        rewind(MKTFILE);
        charArray = (char*)malloc(sizeOfFile);
        sizeOfFile = fread( charArray,1, sizeOfFile, MKTFILE );
        fclose( MKTFILE );

        for(long i = 1; i < sizeOfFile;i++)
        {
            if(charArray[i-1] == '\n')
            {
                int sizeOfFP = 0;
                for(int a = 0; charArray[i+a+1] != '\n' && a+i < sizeOfFile;a++)
                    sizeOfFP = a;
                sizeOfFP++;
                char * FFP = (char*)malloc(librariesPathLength+sizeOfFP+1);
                for(int i = 0; i < librariesPathLength;i++)
                    FFP[i] = librariesPath[i];

                for(int a = 0; a < sizeOfFP; a++)
                    FFP[a+librariesPathLength] = charArray[i+a];
                FFP[librariesPathLength+sizeOfFP] = 0;

                FUNHANDLE hLib = LoadLibrary(FFP);
                DEBUG("loaded");
                if (hLib == NULL) { // not 100% sure if this errorHandling works, so :D
                    DEBUG("error");
                    sizeOfNotLoaded += librariesPathLength+sizeOfFP;
                    notLoaded = (char*)realloc(notLoaded,sizeOfNotLoaded);
                    for(int a = 0; a < sizeOfFP; a++)
                        notLoaded[sizeOfNotLoaded-(librariesPathLength+sizeOfFP)+a-1] = FFP[a];
                } else {
                    Shmodules++;
                    hmodules = (FUNHANDLE*)realloc(hmodules,sizeof(FUNHANDLE) * Shmodules);
                    hmodules[Shmodules-1] = hLib;
                }
            }
        }
    } else {
        printf("\nfile libraryList not found");
    }
    return notLoaded;
}

void * getEntryAddress(FUNHANDLE libraryToLoad)
{
    return (void*)GetProcAddress(libraryToLoad, "entry");
}

#elif __gnu_linux__

void * getEntryAddress(FUNHANDLE * libraryToLoad)
{
    return NULL;
}

#endif