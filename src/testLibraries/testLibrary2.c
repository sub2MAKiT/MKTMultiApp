#include <stdio.h>

void entry(void * ptr)
{
    printf("\nDUCK 2 (loaded a dll) %x\n",ptr);
    return;
}