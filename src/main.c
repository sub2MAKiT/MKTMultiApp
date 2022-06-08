#include "./vulkan/MKTAppEngine.h"
#define MKTGNUBASEDLIBRARY
#define MKT_DEBUG_GUARD
#include "libraryLoader.h"
#include "cmdMode.h"

int main(int argc, char* argv[])
{
    printf("\nboot: %d",argc);
    char cmdMode = checkForCmdMode(argc,argv);

    char * notLoaded = loadLibaries;
    for(int i = 0; i < Shmodules;i++)
    {
        void (*funPtr)() = (void(*)(void))getEntryAddress(hmodules[i]); // so much fun!!!
        (*funPtr)();
    }

    if(cmdMode == 0)
    {
        VentumEngine engine;

        engine.init();

        engine.run();

        engine.cleanup();
    } else if(cmdMode == 1)
    {
        MKTMAcmdMode();
    }
    else if(cmdMode == 2)
        0;

    unloadLibraries

    return 0;
}