#include <stdlib.h>

struct DeletionQueue
{
    void * delQue;
    size_t sizeOfDelQue;

    int * offsetDelQue;
    size_t sizeOfOffsetDelQue;

    void * argv;
    size_t sizeOfArgv;

    void * offsetArgv;
    size_t sizeOfOffsetArgv;
};