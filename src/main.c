#include "./vulkan/MKTAppEngine.h"
#include "libraryLoader.h"

int main(int argc, char* argv[])
{
    char * notLoaded = loadLibaries;

    VentumEngine engine;

    engine.init();

    engine.run();

    printf("everything went fine");

    engine.cleanup();

    return 0;
}