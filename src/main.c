//utils
#include "externDefine.h"
#include "utils.h"
#include "errorHandling.h"

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

    DELQUEVARIABLES
    char a;int b;
    DELQUEVARIABLESDOT(testing)
    input->a,input->b
    DELQUEVARIABLESVALUE
    'a',100
    COMMITDELQUE

    DEBUG("IIII startup IIII");
    if(argc < 3)
        MKTerror(10);

    init();

    run();

    cleanup();

    MKTreturnDelQueue();

    DEBUG("IIII return IIII");

    MKTreturnError("./errors.log");
    return 0;
}