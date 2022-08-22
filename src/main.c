//utils
#include "externDefine.h"
#include "utils.h"

// vulkan
#include <MKTAppEngine.h>

char * testingP1;
char * testingP2;

void testing(char a,int b)
{
    printf("it works %c%d\n",a,b);
}

int main(int argc, char* argv[])
{
    MKTsetupError();
    MKTcreateDelQueue();

    DEBUG("IIII startup IIII");
    if(argc < 3)
        MKTerror(10);

    init();

    run();

    cleanup();

    DEBUG("IIII return IIII");

    return 0;
}