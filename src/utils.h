#pragma once
#include <stdio.h>
#include <stdlib.h>

#ifndef MKTUTILSTHIRD
extern char MKTstrcmp(char * a, char * b);
#else
char MKTstrcmp(char * a, char * b)
{
    for(int i = 0; a[i-1] != 0 && b[i-1] != 0;i++)
        if(a[i] != b[i])
            return false;
    return true;
}
#endif