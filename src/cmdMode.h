#ifdef __gnu_linux__
#include <cstdlib>
#elif _WIN32
#include <stdlib.h>
#include <stdio.h>
#endif
#include "DEBUG.h"

#ifndef cmdModeDirectInclude
extern const char _MKTMA_VERSION[9];
#endif

char checkForCmdMode(int argc, char ** argv);
void  MKTMAcmdMode(void (**funArray)(void*),long sizeOfFunArray);
void _MKTMACMD_help();
void _MKTMACMD_version();
char MKTcompStr(char * charArray, const char * string);
void handleCmdGrapics(char mode);
void _MKTMACMD_copy(char * command);