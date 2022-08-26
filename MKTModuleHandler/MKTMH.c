#include <stdio.h>
#include <stdlib.h>
#include "../src/MKTDLL/dllDefines.h"

void init(initi input)
{
    printf("MKTMH init complete\n");

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
    const char * userOS = "Windows";

    #elif __gnu_linux__
    const char * userOS = "Linux";

    #elif __APPLE__
    const char * userOS = "Mac-os";

    #endif

    printf("result: \nOS: %s\n",userOS);

    return;
}

void cleanUp(cleanUpi input)
{
    printf("MKTMH cleanUp complete\n");

    return;
}