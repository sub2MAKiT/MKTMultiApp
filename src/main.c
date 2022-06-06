#include "./vulkan/MKTAppEngine.h"

int main(int argc, char* argv[])
{
    VentumEngine engine;

    engine.init();

    engine.run();

    printf("everything went fine");

    engine.cleanup();

    return 0;
}