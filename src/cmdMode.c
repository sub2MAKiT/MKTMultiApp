#include "cmdMode.h"

char checkForCmdMode(int argc, char ** argv)
{
    if(argv[1][0] == 'h')
    {
        printf("\nHelp option coming soon\n");
        return 2;
    } else if(argv[1][0] == 'c' && argv[1][1] == 'm' && argv[1][2] == 'd')
        return 1;
    return 0;
}