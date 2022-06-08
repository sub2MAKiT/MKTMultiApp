#include "./vulkan/MKTAppEngine.h"
#define MKTGNUBASEDLIBRARY
#define MKT_DEBUG_GUARD
#include "libraryLoader.h"
#include "cmdMode.h"

int main(int argc, char* argv[])
{
    char cmdMode = checkForCmdMode(argc,argv);
    int tempstep = 0;

    handleCmdGrapics(cmdMode);

    char * notLoaded = loadLibaries;
    void (**funArray)(void*);
    funArray = (void(**)(void*))malloc(sizeof(void (*)(void*)));
    long sizeOfFunArray = 0;
    for(int i = 0; i < Shmodules;i++)
    {
        void * a;
        sizeOfFunArray++;
        funArray = (void(**)(void*))realloc(funArray,sizeof(void (*)(void*))*sizeOfFunArray);
        funArray[sizeOfFunArray-1] = (void(*)(void*))getEntryAddress(hmodules[i]); // so much fun!!!
        funArray[sizeOfFunArray-1](a);
    }
    if(cmdMode == 0)
    {
        VentumEngine engine;

        engine.init();

        engine.run();

        engine.cleanup();
    } else if(cmdMode == 1)
    {
        MKTMAcmdMode(funArray, sizeOfFunArray);
    }
    else if(cmdMode == 2)
        0;

    unloadLibraries

    return 0;
}