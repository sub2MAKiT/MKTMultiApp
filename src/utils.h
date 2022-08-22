#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <extern.h>

unsigned char MKTstrcmp(char * a, char * b);
unsigned char weakMKTstrcmp(char * a, char * b);
unsigned char MKTCstrcmp(const char * a, char * b);

#define MKTDOUBLE double

long long MKTfloor(MKTDOUBLE input);
#ifdef MKT_DEBUG
#define DEBUG(x)              \
printf("\033[%d;40m[%d]" x "\033[0;40m\n",(int)((_step % 6) + 1 + MKTfloor((float)(((_step % 2) + 1)*1.5)) * 30),_step);_step++
//assert(x);printf("\n");step++
#else
#define DEBUG(x)                                                \
0
#endif

#ifdef MKT_R_DEBUG
#define RDEBUG(x)              \
printf("\033[%d;40m[%d]" x "\033[0;40m\n",(int)((_step % 6) + 1 + MKTfloor((float)(((_step % 2) + 1)*1.5)) * 30),_step);_step++
//assert(x);printf("\n");step++
#else
#define RDEBUG(x)                                                \
0
#endif