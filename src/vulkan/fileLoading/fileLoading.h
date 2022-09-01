#define MKTINFOV 0
#define MKTAGV 1
#define MKTAUDIOV 2
#define MKTPICV 3
#define MKTVIDV 4

#include <stdio.h>
#include <stdlib.h>
#include <fileLoading/AG.h>
#include <fileLoading/Pic.h>
#include <fileLoading/Audio.h>
#include <fileLoading/Info.h>
#include <fileLoading/ViD.h>

IntDex loadFile(char * FP, char type);