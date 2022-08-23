//utils
#include "externDefine.h"
#include "utils.h"

// vulkan
#include <MKTAppEngine.h>

char * testingP1;
char * testingP2;

int testing(char a,int b)
{
    printf("it works %c%d\n",a,b);
    return 0;
}

int main(int argc, char* argv[])
{
    MKTsetupError();
    MKTcreateDelQueue();

    DEBUG("IIII startup IIII");

    init();

    run();

    cleanup();

    DEBUG("IIII return IIII");

    return 0;
}