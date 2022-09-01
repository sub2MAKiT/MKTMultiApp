//utils
#include "externDefine.h"
#include "utils.h"

#define MKTDYNAMICLIBRARYLOADING
#include <MKTAppEngine.h>
#include <../MKTDLL/MKTDLL.h>

int main(int argc, char* argv[])
{
    MKTsetupError();
    // MKTcreateDelQueue();

    // loadLibaries;

    DEBUG("IIII startup IIII");

    init();

    run();

    cleanup();

    // unloadLibraries;

    DEBUG("IIII return IIII");

    return 0;
}