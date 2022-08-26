#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <../utils.h>
#include <../MKTDLL/dllDefines.h>
#include <../outerDefine.h>

typedef struct MKTMODULE {
    void (*init)(initi);
    void (*run)(funi);
    void (*cleanUp)(cleanUpi);
    char * name;
    IntDex sizeOfName;
} MKTmodule;

#ifdef __gnu_linux__
#define FUNHANDLE void *
#define librariesPathLength 12

#elif _WIN32
#include <windows.h>
#define FUNHANDLE HMODULE
#define WIN32_LEAN_AND_MEAN
#define librariesPathLength 14

#elif __APPLE__

#endif

#ifdef MKTDYNAMICLIBRARYLOADING // header guard
MKTmodule * _MKT_MODULES;
IntDex _MKT_sizeOfMODULES;
#else
extern MKTmodule * _MKT_MODULES;
extern IntDex _MKT_sizeOfMODULES;
#endif // MKTDYNAMICLIBRARYLOADING

#ifndef MKT_DLL_LOADING
extern long Shmodules; //  modules, more like shmodules am i right?
extern FUNHANDLE * hmodules;
#ifdef __gnu_linux__
// for linux

void  *dlopen(const char *, int);
void  *dlsym(void *, const char *);
int    dlclose(void *);
char  *dlerror(void);

MKTmodule getEntryAddress(FUNHANDLE libraryToLoad);
#define loadLibaries 0;
#define getEntryInLibrary(x) 0;
#define unloadLibraries 0;

#elif _WIN32
// for windows

char * windowsDLLLoading();
#define unloadLibraries cleanUpi tempCleanUpI; tempCleanUpI.errorCode = _errors; for(int i = 0; i < Shmodules;i++)(*_MKT_MODULES[i].cleanUp)(tempCleanUpI); for(int i = 0; i < Shmodules;i++)FreeLibrary(hmodules[i])

void getEntryAddress(FUNHANDLE libraryToLoad,MKTmodule * Module);
#define loadLibaries windowsDLLLoading();\
for(int i = 0; i < _MKT_sizeOfMODULES;i++)\
getEntryAddress(hmodules[i],&_MKT_MODULES[i]);



#elif __APPLE__
// for, you guessed it, apple

#define loadLibaries 0;
#define getEntryInLibrary(x) 0;
#define unloadLibraries 0;

#endif // OS
#endif // MKTDYNAMICLIBRARYLOADING