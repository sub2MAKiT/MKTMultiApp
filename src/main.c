//utils
#include "externDefine.h"
#include "utils.h"
#include "errorHandling.h"

// vulkan
#include <MKTAppEngine.h>


int main(int argc, char* argv[])
{
    MKTsetupError();
    DEBUG("IIII startup IIII");
    if(argc < 3)
        MKTerror(10);

    init();

    run();

    cleanup();

    DEBUG("IIII return IIII");
    MKTreturnError("./errors.log");
    return 0;
}