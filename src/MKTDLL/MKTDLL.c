#define MKT_DLL_LOADING
#include <../MKTDLL/MKTDLL.h>
#include <../errorHandling.h>

// long Shmodules = 0;
// FUNHANDLE * hmodules = NULL;

// long fileShmodules = 0;
// FUNHANDLE * fileHmodules = NULL;

#ifdef _WIN32

#define CURRENTF _MKT_sizeOfFileModules-1

void windowsFileDLLLoading()
{
    FILE * MKTFILE = fopen("./files/files.MKTI","rb");

    fileShmodules = 0;
    SAFEMALLOC(fileHmodules,sizeof(FUNHANDLE));
    _MKT_sizeOfFileModules = 0;
    SAFEMALLOC(_MKT_fileModules,sizeof(MKTfileModule));

    if(MKTFILE != NULL)
    {

        char *charArray;
        fseek(MKTFILE, 0L, SEEK_END);
        long sizeOfFile = ftell(MKTFILE);
        rewind(MKTFILE);
        charArray = (char*)malloc(sizeOfFile);
        sizeOfFile = fread( charArray,1, sizeOfFile, MKTFILE );
        fclose( MKTFILE );

        for(int i = 0; i < sizeOfFile; i++)
        {
            if(i == 0 || charArray[i-1] == '\n')
            {
                char buffer[100]; // in theory hardcoding this is a bad idea, in practice, realloc shenanigans would just slow it down.

                _MKT_sizeOfFileModules++;
                _MKT_fileModules = realloc(_MKT_fileModules,sizeof(MKTfileModule)*_MKT_sizeOfFileModules);

                _MKT_fileModules[CURRENTF].FileName = malloc(1);

                for(int a = 0; charArray[i+a-1] != ' '; a++)
                {
                    _MKT_fileModules[CURRENTF].sizeOfFileName = a+1;
                    _MKT_fileModules[CURRENTF].FileName = realloc(_MKT_fileModules[CURRENTF].FileName,_MKT_fileModules[CURRENTF].sizeOfFileName);
                    _MKT_fileModules[CURRENTF].FileName[a] = charArray[i+a]!=' '?charArray[i+a]:0;
                }

                snprintf(buffer,100,"./files/%s.dll",_MKT_fileModules[CURRENTF].FileName);
                fileShmodules++;
                fileHmodules = realloc(fileHmodules,fileShmodules*sizeof(FUNHANDLE));
                fileHmodules[fileShmodules-1] = LoadLibrary(buffer);

                typedef struct MKTFILEMODULE {
                char type;
                } MKTfileModule;

                _MKT_fileModules[CURRENTF].load = (MKTInfo*(*)(char * FP))GetProcAddress(fileHmodules[fileShmodules-1], "load");
                _MKT_fileModules[CURRENTF].init = (char(*)(char))GetProcAddress(fileHmodules[fileShmodules-1], "init");
                _MKT_fileModules[CURRENTF].type = (*_MKT_fileModules[CURRENTF].init)(
                    #ifdef MKT_DEBUG
                    0b00000001
                    #else
                    0b00000000
                    #endif
                    );

                }
        }

    } else
        MKTerror(2);

    return;
}

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

void getEntryAddress(FUNHANDLE libraryToLoad, MKTmodule * Module)
{
    Module->init = (void(*)(initi))GetProcAddress(libraryToLoad, "init");
    Module->run = (void(*)(funi))GetProcAddress(libraryToLoad, "run");
    Module->cleanUp = (void(*)(cleanUpi))GetProcAddress(libraryToLoad, "cleanUp");
    return;
}

#elif __gnu_linux__

#include <dlfcn.h>

// void  *dlopen(const char *, int);
// void  *dlsym(void *, const char *);
// int    dlclose(void *);
// char  *dlerror(void);

void * loadFileSharedObjects()
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

void * loadSharedObjects()
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
                    // sizeOfNotLoaded += librariesPathLength+sizeOfFP;
                    // notLoaded = (char*)realloc(notLoaded,sizeOfNotLoaded);
                    // for(int a = 0; a < sizeOfFP; a++)
                        // notLoaded[sizeOfNotLoaded-(librariesPathLength+sizeOfFP)+a-1] = FFP[a];
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

void getEntryAddress(FUNHANDLE libraryToLoad, MKTmodule * Module)
{
    Module->init = (void(*)(initi))dlsym(libraryToLoad, "init");
    Module->run = (void(*)(funi))dlsym(libraryToLoad, "run");
    Module->cleanUp = (void(*)(cleanUpi))dlsym(libraryToLoad, "cleanUp");
    return;
}

#endif