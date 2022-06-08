#include <stdio.h>

void entry(void * ptr)
{
    printf("\nDUCK (loaded a dll) %x\n",ptr);
    return;
}