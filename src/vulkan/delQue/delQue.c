#include <delQue/delQue.h>
#include <extern.h>
#include <stdio.h>

// #define delQueGenVar(type,...)##__VA_ARGS__

char * initDelQue;

void MKTcreateDelQueue()
{
    initDelQue = (char*)malloc(3);
    _mainDelQue.sizeOfDelQue = 1;
    _mainDelQue.delQue = malloc(8);
    _mainDelQue.delQue[0].MKTDelQueueInputFunction = (void(*)(void *))&free;

    _mainDelQue.sizeOfArgc = 1;
    _mainDelQue.argc = (DelQueArgv*)malloc(_mainDelQue.sizeOfArgc*sizeof(DelQueArgv));
    _mainDelQue.argc[0].argv = initDelQue;
}

void MKTaddDelQueue(void * function,void * argv,IntDex sizeOfArgv)
{
    _mainDelQue.sizeOfDelQue++;
    _mainDelQue.delQue = realloc(_mainDelQue.delQue,8*_mainDelQue.sizeOfDelQue);
    _mainDelQue.delQue[_mainDelQue.sizeOfDelQue-1].MKTDelQueueInputFunction = (void(*)(void *))function;

    _mainDelQue.sizeOfArgc++;
    _mainDelQue.argc = (DelQueArgv*)realloc(_mainDelQue.argc,_mainDelQue.sizeOfArgc*sizeof(DelQueArgv));
    _mainDelQue.argc[_mainDelQue.sizeOfArgc-1].argv = malloc(sizeOfArgv);
    for(unsigned int i = 0; i < sizeOfArgv;i++)
        _mainDelQue.argc[_mainDelQue.sizeOfArgc-1].argv[i] = *(char*)(argv+i);
}

void MKTreturnDelQueue()
{
    for(IntDex i = 0; i < _mainDelQue.sizeOfDelQue; i++)
        (*_mainDelQue.delQue[i].MKTDelQueueInputFunction)(_mainDelQue.argc[i].argv);
        

}