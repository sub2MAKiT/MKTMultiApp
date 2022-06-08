#include <stdio.h>
#include <stdlib.h>
#include "./DEBUG.h"

#ifndef MKTDYNAMICLIBRARYLOADING // header guard
#define MKTDYNAMICLIBRARYLOADING
#ifdef __gnu_linux__
// for linux
#define FUNHANDLE char

void * getEntryAddress(FUNHANDLE * libraryToLoad);

#define loadLibaries 0;
#define getEntryInLibrary(x) 0;
#define unloadLibraries 0;

#elif _WIN32
// for windows
#define FUNHANDLE HMODULE
#include <windows.h>
#define WIN32_LEAN_AND_MEAN

char * windowsDLLLoading();

#define unloadLibraries for(int i = 0; i < Shmodules;i++)FreeLibrary(hmodules[i]);
void * getEntryAddress(FUNHANDLE libraryToLoad);

#define loadLibaries windowsDLLLoading()

#define librariesPathLength 14
//--------------
#define libraryStartingPoint(x) BOOL APIENTRY DllMain( FUNHANDLE hModule, \
                       DWORD  ul_reason_for_call,\
                       LPVOID lpReserved\
                     )\
{\
    switch (ul_reason_for_call)\
    {\
    case DLL_PROCESS_ATTACH:\
        x;\
        DEBUG("Library loaded\n");\
        break;\
    case DLL_THREAD_ATTACH:\
        DEBUG("Thread attached\n");\
        break;\
    case DLL_THREAD_DETACH:\
        DEBUG("Thread detached\n");\
        break;\
    case DLL_PROCESS_DETACH:\
        DEBUG("Process detached\n");\
        break;\
    }\
    return TRUE;\
}


//--------------
#elif __APPLE__
// for, you guessed it, apple

#define loadLibaries 0;
#define getEntryInLibrary(x) 0;
#define unloadLibraries 0;

#endif // OS
#endif // MKTDYNAMICLIBRARYLOADING

#ifndef MKT_DLL_LOADING
extern long Shmodules;
extern FUNHANDLE hmodules;
#endif