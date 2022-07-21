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
    MKTFILE = fopen( "./lib/libraryList.MKTI", "rb" );
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
            if(charArray[i-1] == '\n' && charArray[i-2] != '\n' && charArray[i] != '{')
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

                const char extension[5] = ".dll";
                sizeOfFP += 4;

                FFP = (char*)realloc(FFP,sizeOfFP);

                for(int a = 0; a < 4;a++)
                    FFP[librariesPathLength+sizeOfFP-4+a] = extension[a];
                
                FFP[librariesPathLength+sizeOfFP] = 0;

                printf("\n%s",FFP);

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

void getEntryAddress(FUNHANDLE libraryToLoad, GL * Module)
{
    Module->init = (void(*)(initi))GetProcAddress(libraryToLoad, "init");
    Module->run = (void(*)(funi))GetProcAddress(libraryToLoad, "run");
    Module->cleanUp = (void(*)(cleanUpi))GetProcAddress(libraryToLoad, "cleanUp");
    return;
}

#elif __gnu_linux__

#include <dlfcn.h>

void * getEntryAddress(FUNHANDLE libraryToLoad)
{
    char librariesPath[13] = "./lib/linux/";
    hmodules = (FUNHANDLE*)malloc(sizeof(FUNHANDLE));
    FILE *MKTFILE;
    char * notLoaded = (char*)malloc(1);
    size_t sizeOfNotLoaded = 0;
    MKTFILE = fopen( "./lib/libraryList.MKTI", "rb" );
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

                char*TFFP=(char*)malloc(sizeOfFP);
                for(int a = 0; a < sizeOfFP;a++)
                    TFFP[a] = FFP[a+librariesPathLength];

                free(TFFP);

                const char extension[4] = ".so";

                sizeOfFP += 3;

                FFP = (char*)realloc(FFP,sizeOfFP);


                for(int a = 0; a < 3;a++)
                    FFP[librariesPathLength+sizeOfFP-3+a] = extension[a];
                
                FFP[librariesPathLength+sizeOfFP] = 0;

                FUNHANDLE hLib = dlopen(FFP,RTLD_NOW);
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

    return NULL;
}

#endif