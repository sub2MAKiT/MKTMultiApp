#pragma once
#include <stdlib.h>
#include <stdarg.h>
#include "../../outerDefine.h"

typedef struct DeletionQueueArguments {
    char * argv;
} DelQueArgv;

typedef struct delQueFunction {
    void (*MKTDelQueueInputFunction)(void * input);
} delQueFun;

typedef struct DeletionQueue {
    delQueFun * delQue;
    IntDex sizeOfDelQue;

    DelQueArgv * argc;
    IntDex sizeOfArgc;
} MKTDelQue;

void MKTcreateDelQueue();
void MKTaddDelQueue(void * function,void * argv,IntDex sizeOfArgv);
void MKTreturnDelQueue();


#define DELQUEVARIABLES    {struct tempStruct{
#define DELQUEVARIABLESDOT(function)        };void tempFunction(struct tempStruct * input){function(
#define DELQUEVARIABLESVALUE            );}struct tempStruct tempInput = {
#define COMMITDELQUE        };MKTaddDelQueue(&tempFunction,&tempInput,sizeof(struct tempStruct));}

// USAGE:
    // DELQUEVARIABLES
    // char a;int b;
    // DELQUEVARIABLESDOT(testing)
    // input->a,input->b
    // DELQUEVARIABLESVALUE
    // 'a',100
    // COMMITDELQUE