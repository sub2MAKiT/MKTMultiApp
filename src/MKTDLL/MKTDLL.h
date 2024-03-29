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

#ifdef _WIN32
#include <windows.h>
#define WIN32_LEAN_AND_MEAN
#define FUNHANDLE HMODULE
#elif __gnu_linux__
#define FUNHANDLE void *
#endif

#ifndef MKTDYNAMICLIBRARYLOADING // header guard
#define MKTDYNAMICLIBRARYLOADING
extern long Shmodules; //  modules, more like shmodules am i right?
extern FUNHANDLE * hmodules;
extern long fileShmodules;
extern FUNHANDLE * fileHmodules;
extern MKTmodule * _MKT_modules;
extern IntDex _MKT_sizeOfModules;
extern MKTfileModule * _MKT_fileModules;
extern IntDex _MKT_sizeOfFileModules;
#ifdef __gnu_linux__
// for linux
#define librariesPathLength 12

#elif _WIN32
// for windows
#define librariesPathLength 14

#elif __APPLE__
// for, you guessed it, apple

#endif // OS
#else
MKTmodule * _MKT_modules;
IntDex _MKT_sizeOfModules;
MKTfileModule * _MKT_fileModules;
IntDex _MKT_sizeOfFileModules;
long Shmodules = 0; //  modules, more like shmodules am i right?
FUNHANDLE * hmodules = NULL;
long fileShmodules = 0;
FUNHANDLE * fileHmodules = NULL;
#endif // MKTDYNAMICLIBRARYLOADING

#ifdef __gnu_linux__
// for linux

// #include <dlfcn.h>

void getEntryAddress(FUNHANDLE libraryToLoad,MKTmodule * Module);
void fileSOLoading();
void * loadSharedObjects();
#define getEntryInLibrary(x) 0;

#define loadLibaries fileSOLoading(); loadSharedObjects(); SAFEMALLOC(_MKT_modules,sizeof(MKTmodule)*Shmodules);for(int i = 0; i < Shmodules; i++) getEntryAddress(hmodules[i],&_MKT_modules[i]);_MKT_sizeOfModules = Shmodules;initi tempInit = {10,10};for(int i = 0; i < _MKT_sizeOfModules;i++) (*_MKT_modules[i].init)(tempInit);
#define unloadLibraries cleanUpi tempCleanUp = {}; for(int i = 0; i < _MKT_sizeOfModules; i++) (*_MKT_modules[i].cleanUp)(tempCleanUp);free(_MKT_modules)


#elif _WIN32
// for windows
void getEntryAddress(FUNHANDLE libraryToLoad,MKTmodule * Module);
void windowsDLLLoading();
void windowsFileDLLLoading();

#define loadLibaries windowsFileDLLLoading(); windowsDLLLoading(); SAFEMALLOC(_MKT_modules,sizeof(MKTmodule)*Shmodules);for(int i = 0; i < Shmodules; i++) getEntryAddress(hmodules[i],&_MKT_modules[i]);_MKT_sizeOfModules = Shmodules;initi tempInit = {10,10};for(int i = 0; i < _MKT_sizeOfModules;i++) (*_MKT_modules[i].init)(tempInit);
#define unloadLibraries cleanUpi tempCleanUp = {}; for(int i = 0; i < _MKT_sizeOfModules; i++) (*_MKT_modules[i].cleanUp)(tempCleanUp); for(int i = 0; i < Shmodules;i++)FreeLibrary(hmodules[i]); free(_MKT_modules)

#elif __APPLE__
// for, you guessed it, apple

#define loadLibaries 0;
#define getEntryInLibrary(x) 0;
#define unloadLibraries 0;

#endif // OS