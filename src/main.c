#include "./vulkan/MKTAppEngine.h"
#define MKTGNUBASEDLIBRARY
#define MKT_DEBUG_GUARD
#include "DEBUG.h"
#include "libraryLoader.h"
#include "cmdMode.h"

GL * Modules = (GL*)malloc(sizeof(GL));
size_t sizeOfModules;

int main(int argc, char* argv[])
{
    char cmdMode = checkForCmdMode(argc,argv);
    int tempstep = 0;

    handleCmdGrapics(cmdMode);

    char * notLoaded = loadLibaries;

    sizeOfModules = Shmodules;

    for(int i = 0; i < sizeOfModules;i++)
        Modules[i].entry = (void(*)(funi))getEntryAddress(hmodules[i]); // so much fun!!!

        // funArray[sizeOfFunArray-1](a);
    if(cmdMode == 0)
    {
        init();

        run();

        cleanup();
    } else if(cmdMode == 1)
    {
        MKTMAcmdMode(Modules, sizeOfModules);
    }
    else if(cmdMode == 2)
        0;

    unloadLibraries

    return 0;
}