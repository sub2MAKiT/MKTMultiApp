#include "./vulkan/MKTAppEngine.h"
#define MKTGNUBASEDLIBRARY
#define MKT_DEBUG_GUARD
#include "DEBUG.h"
#include "libraryLoader.h"
#include "cmdMode.h"

GL * Modules;
size_t sizeOfModules;

int main(int argc, char* argv[])
{
    char cmdMode = checkForCmdMode(argc,argv);
    int tempstep = 0;

    handleCmdGrapics(cmdMode);

    char * notLoaded = loadLibaries;

    sizeOfModules = Shmodules;

    Modules = (GL*)malloc(sizeof(GL)*Shmodules);

    for(int i = 0; i < sizeOfModules;i++)
        getEntryAddress(hmodules[i],&Modules[i]); // so much fun!!!

    DEBUG("init init");

        // funArray[sizeOfFunArray-1](a);
    if(cmdMode == 0)
    {
        DEBUG("started graphical mode");

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