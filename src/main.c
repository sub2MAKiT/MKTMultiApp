#include "utils.h"
#include <MKTAppEngine.h>
#include "libraryLoader.h"
#include "cmdMode.h"
#include "externDefine.h"

int main(int argc, char* argv[])
{
    DEBUG("IIII startup IIII");
    init();

    run();

    cleanup();
    DEBUG("IIII return IIII");
    return 0;
}