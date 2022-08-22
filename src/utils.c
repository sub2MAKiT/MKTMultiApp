#include "utils.h"
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

unsigned char MKTCstrcmp(const char * a, char * b)
{
    unsigned int i = 0;
    while(a[i++]==b[i])
        if(a[i]==0)
            return b[i]==a[i];
    return 0;
}
unsigned char weakMKTCstrcmp(const char * a, char * b)
{
    unsigned int i = 0;
    while(a[++i]==b[i])
        if(a[i]==0||b[i]==0)
            return 1;
    return 0;
}

long long MKTfloor(MKTDOUBLE input)
{
    long long returnValue = input;
    return returnValue;
}

unsigned int MKTClamp(unsigned int a,unsigned int b,unsigned int c)
{
    return a>c?c:(a<b?b:a);
}
