#pragma once
#include <stdio.h>
#include <stdlib.h>

#ifndef MKTUTILSTHIRD
extern char MKTstrcmp(char * a, char * b);
extern char weakMKTstrcmp(char * a, char * b);
#else
unsigned char MKTstrcmp(char * a, char * b)
{
    unsigned int i = 0;
    while(a[i++]==b[i])
        if(a[i]==0)
            return b[i]==a[i];
    return 0;
}
unsigned char weakMKTstrcmp(char * a, char * b)
{
    unsigned int i = 0;
    while(a[++i]==b[i])
        if(a[i]==0||b[i]==0)
            return 1;
    return 0;
}
#endif