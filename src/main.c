#include <MKTAppEngine.h>
#include "libraryLoader.h"
#include "cmdMode.h"


int main(int argc, char* argv[])
{
    init();

    run();

    cleanup();
    
    return 0;
}