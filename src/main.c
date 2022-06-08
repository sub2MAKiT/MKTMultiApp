#include "./vulkan/MKTAppEngine.h"
#define MKTGNUBASEDLIBRARY
#define MKT_DEBUG_GUARD
#include "libraryLoader.h"

int main(int argc, char* argv[])
{
    char * notLoaded = loadLibaries;
    printf("\ntest dlls %d",Shmodules);
    for(int i = 0; i < Shmodules;i++)
    {
        void (*funPtr)() = (void(*)(void))getEntryAddress(hmodules[i]); // so much fun!!!
        (*funPtr)();
    }
    VentumEngine engine;

    engine.init();

    engine.run();

    printf("everything went fine");

    engine.cleanup();

    unloadLibraries

    return 0;
}