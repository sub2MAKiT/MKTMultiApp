#include "libraryLoader.h"

extern void test();

libraryStartingPoint(test())

void test()
{
    printf("\nDUCK (loaded a dll)\n");
    return;
}