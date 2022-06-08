#include "./vulkan/MKTAppEngine.h"
#ifdef __gnu_linux__
#include <cstdlib>
#endif
#define MKTGNUBASEDLIBRARY
#define MKT_DEBUG_GUARD
#include "libraryLoader.h"
#include "cmdMode.h"

int main(int argc, char* argv[])
{
    // char cmdMode = checkForCmdMode(argc,argv);
    char cmdMode = 0;

    char * notLoaded = loadLibaries;
    for(int i = 0; i < Shmodules;i++)
    {
        void (*funPtr)() = (void(*)(void))getEntryAddress(((char*)hmodules)+i); // so much fun!!!
        (*funPtr)();
    }

    if(cmdMode == 0)
    {
        VentumEngine engine;

        engine.init();

        engine.run();

        engine.cleanup();
    } else if(cmdMode == 1)
        printf("\nCmd mode coming soon!!!");
    else if(cmdMode == 2)
        0;

    unloadLibraries

    return 0;
}