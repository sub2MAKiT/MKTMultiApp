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

typedef struct MKTFILEMODULE {
    MKTInfo * (*load)(char * FP);
    char (*init)(char input);
    char * FileName;
    IntDex sizeOfFileName;
    char type;
} MKTfileModule;

void loadMenuAG(char * MKTAGOGName, size_t sizeOfOGName,int moduleNumber);

#ifndef MKTDYNAMICLIBRARYLOADING // header guard
#define MKTDYNAMICLIBRARYLOADING
extern MKTmodule * _MKT_modules;
extern IntDex _MKT_sizeOfModules;
extern MKTfileModule * _MKT_fileModules;
extern IntDex _MKT_sizeOfFileModules;
#ifdef __gnu_linux__
// for linux
#define FUNHANDLE void *
#define librariesPathLength 12

#elif _WIN32
// for windows
#include <windows.h>
#define WIN32_LEAN_AND_MEAN
#define librariesPathLength 14

#elif __APPLE__
// for, you guessed it, apple

#endif // OS
#else
MKTmodule * _MKT_modules;
IntDex _MKT_sizeOfModules;
MKTfileModule * _MKT_fileModules;
IntDex _MKT_sizeOfFileModules;
#endif // MKTDYNAMICLIBRARYLOADING
#ifdef _WIN32
#define FUNHANDLE HMODULE
#endif

#ifndef MKT_DLL_LOADING
extern long Shmodules; //  modules, more like shmodules am i right?
extern FUNHANDLE * hmodules;
#ifdef __gnu_linux__
// for linux

void  *dlopen(const char *, int);
void  *dlsym(void *, const char *);
int    dlclose(void *);
char  *dlerror(void);

GL getEntryAddress(FUNHANDLE libraryToLoad);
#define loadLibaries 0;
#define getEntryInLibrary(x) 0;
#define unloadLibraries 0;

#elif _WIN32
// for windows
void getEntryAddress(FUNHANDLE libraryToLoad,MKTmodule * Module);
char * windowsDLLLoading();
void windowsFileDLLLoading();

#define loadLibaries windowsFileDLLLoading(); windowsDLLLoading(); SAFEMALLOC(_MKT_modules,sizeof(MKTmodule)*Shmodules);for(int i = 0; i < Shmodules; i++) getEntryAddress(hmodules[i],&_MKT_modules[i]);_MKT_sizeOfModules = Shmodules;initi tempInit = {10,10};for(int i = 0; i < _MKT_sizeOfModules;i++) (*_MKT_modules[i].init)(tempInit);
#define unloadLibraries cleanUpi tempCleanUp = {}; for(int i = 0; i < _MKT_sizeOfModules; i++) (*_MKT_modules[i].cleanUp)(tempCleanUp); for(int i = 0; i < Shmodules;i++)FreeLibrary(hmodules[i]); free(_MKT_modules)

#elif __APPLE__
// for, you guessed it, apple

#define loadLibaries 0;
#define getEntryInLibrary(x) 0;
#define unloadLibraries 0;

#endif // OS
#endif // MKTDYNAMICLIBRARYLOADING