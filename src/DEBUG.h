#ifndef MKT_DEBUG_THIRD_GUARD
#define MKT_DEBUG_THIRD_GUARD
#define MKTDOUBLE double
#ifdef MKT_DEBUG_GUARD
int step = 0;

long long MKTfloor(MKTDOUBLE input)
{
    long long returnValue = input;
    return returnValue;
}

#else
extern long long MKTfloor(MKTDOUBLE input);
extern int step;
#endif
#ifdef MKT_DEBUG
#define DEBUG(x)              \
printf("\033[%d;40m[%d]" x "\033[0;40m\n",int((step % 6) + 1 + MKTfloor(float(((step % 2) + 1)*1.5)) * 30),step);step++
//assert(x);printf("\n");step++
#else
#define DEBUG(x)                                                \
0
#endif
#endif