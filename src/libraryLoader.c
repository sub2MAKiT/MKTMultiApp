#include "libraryLoader.h"

char * windowsDLLLoading()
{
    char * notLoaded = (char*)malloc(1);
    HMODULE hLib = LoadLibrary("./lib/windows/testLibrary.dll");

    if (hLib == NULL) {
        printf("Library testLibrary.dll not found\n");
        exit(1);
    }
    return notLoaded;
}