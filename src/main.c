#include "./vulkan/MKTAppEngine.h"
#define MKTGNUBASEDLIBRARY
#define MKT_DEBUG_GUARD
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
        Modules[i].entry = (void(*)(void*))getEntryAddress(hmodules[i]); // so much fun!!!

        // funArray[sizeOfFunArray-1](a);
    if(cmdMode == 0)
    {
        VentumEngine engine;

        engine.init();

        engine.run();

        engine.cleanup();
    } else if(cmdMode == 1)
    {
        MKTMAcmdMode(Modules, sizeOfModules);
    }
    else if(cmdMode == 2)
        0;

    unloadLibraries

    return 0;
}