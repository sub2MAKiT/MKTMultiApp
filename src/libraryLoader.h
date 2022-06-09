#include <stdio.h>
#include <stdlib.h>
#include "./DEBUG.h"

#ifndef MKTDYNAMICLIBRARYLOADING // header guard
#define MKTDYNAMICLIBRARYLOADING
#ifdef __gnu_linux__
// for linux
#define FUNHANDLE void *
#define librariesPathLength 12

#elif _WIN32
// for windows
#include <windows.h>
#define FUNHANDLE HMODULE
#define WIN32_LEAN_AND_MEAN
#define librariesPathLength 14

#elif __APPLE__
// for, you guessed it, apple

#endif // OS
#endif // MKTDYNAMICLIBRARYLOADING

#ifndef MKT_DLL_LOADING
extern long Shmodules;
extern FUNHANDLE * hmodules;
#ifdef __gnu_linux__
// for linux

void  *dlopen(const char *, int);
void  *dlsym(void *, const char *);
int    dlclose(void *);
char  *dlerror(void);

void * getEntryAddress(FUNHANDLE libraryToLoad);
#define loadLibaries 0;
#define getEntryInLibrary(x) 0;
#define unloadLibraries 0;

#elif _WIN32
// for windows

char * windowsDLLLoading();
#define unloadLibraries for(int i = 0; i < Shmodules;i++)FreeLibrary(hmodules[i]);
void * getEntryAddress(FUNHANDLE libraryToLoad);
#define loadLibaries windowsDLLLoading()

#elif __APPLE__
// for, you guessed it, apple

#define loadLibaries 0;
#define getEntryInLibrary(x) 0;
#define unloadLibraries 0;

#endif // OS
#endif // MKTDYNAMICLIBRARYLOADING