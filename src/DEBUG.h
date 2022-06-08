#ifndef MKT_DEBUG_GUARD
#define MKT_DEBUG_GUARD
#ifdef MKT_DEBUG
#define DEBUG(x)                                                \
printf("\n\033[%dm[%d]" x "\033[0m\n",int((rand() % 6) + 1 + floor(float(((rand() % 2) + 1)*1.5)) * 30),int((rand() % 6) + 1 + floor(float(((rand() % 2) + 1)*1.5)) * 30));
#else
#define DEBUG(x)                                                \
0;
#endif
#endif