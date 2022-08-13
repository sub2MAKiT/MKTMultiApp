#include <stdio.h>
#include <stdlib.h>
#include "../src/libraryHeader.h"

void init(initi input)
{

    return;
}

char MKTMH_runBasicCheck()
{
    char checkResult = 0;
    FILE * BASICCHECK = fopen(
#ifdef _WIN32
"main.exe"
#elif __gnu_linux__
"main"
#elif __APPLE__
#endif
,"rb");
    checkResult = BASICCHECK==NULL;
    fclose(BASICCHECK);
    return checkResult;
}

void run(funi input)
{
    #ifdef _WIN32
    char _USEROS_ = 0;

    #elif __gnu_linux__
    char _USEROS_ = 1;

    #elif __APPLE__
    char _USEROS_ = 2;

    #endif

    return;
}

void cleanUp(cleanUpi input)
{

    return;
}