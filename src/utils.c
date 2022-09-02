#include "utils.h"
unsigned char MKTstrcmp(char * a, char * b)
{
    unsigned int i = 0;
    while(a[++i]==b[i])
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

char * MKTdissectFileType(unsigned long long int * size, char * string)
{
    unsigned long long int tempSize1 = 0;
    unsigned long long int tempSize2 = 0;
    for(int i = 0; string[i] != 0; i++)
        tempSize1 = i;
    tempSize2 = ++tempSize1;
    while(string[--tempSize1-1]!=46)0;
    char * returnString = malloc(tempSize2-tempSize1+1);
    *size = tempSize2-tempSize1;
    for(int i = tempSize1;i <= tempSize2;i++)returnString[i-tempSize1] = string[i];
    returnString[tempSize2-tempSize1+1] = 0;
    return returnString;
}