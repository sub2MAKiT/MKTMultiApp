#ifndef MKT_DEBUG_THIRD_GUARD
#define MKT_DEBUG_THIRD_GUARD
#ifdef MKT_DEBUG_GUARD
int step = 0;
#else
extern int step;
#endif
#ifdef MKT_DEBUG
#include <MKTmisc/MKTGNUbasedHeaders/MKTmath.h>
#define DEBUG(x)              \
printf_s("\033[%d;40m[%d]" x "\033[0;40m\n",int((step % 6) + 1 + MKTfloor(float(((step % 2) + 1)*1.5)) * 30),step);step++
//assert(x);printf("\n");step++
#else
#define DEBUG(x)                                                \
0
#endif
#endif