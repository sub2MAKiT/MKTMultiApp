#include <delQue/delQue.h>
#include <extern.h>
#include <stdio.h>

char * initDelQue;

void MKTcreateDelQueue()
{
    initDelQue = (char*)malloc(3);
    _mainDelQue.sizeOfDelQue = 0;
    _mainDelQue.sizeOfArgc = 0;
}

void MKTaddDelQueue(void * function,void * argv,IntDex sizeOfArgv)
{
    _mainDelQue.sizeOfDelQue++;
    _mainDelQue.delQue = realloc(_mainDelQue.delQue,8*_mainDelQue.sizeOfDelQue);
    _mainDelQue.delQue[_mainDelQue.sizeOfDelQue-1].MKTDelQueueInputFunction = (void(*)(void *))function;

    _mainDelQue.sizeOfArgc++;
    _mainDelQue.argc = (DelQueArgv*)realloc(_mainDelQue.argc,_mainDelQue.sizeOfArgc*sizeof(DelQueArgv));
    _mainDelQue.argc[_mainDelQue.sizeOfArgc-1].argv = malloc(sizeOfArgv);
    for(IntDex i = 0; i < sizeOfArgv; i++)
    _mainDelQue.argc[_mainDelQue.sizeOfArgc-1].argv[i] = *(char*)(argv+i);
}

void MKTreturnDelQueue()
{
    for(IntDex i = 0; i < _mainDelQue.sizeOfDelQue; i++)
    {
        (*_mainDelQue.delQue[i].MKTDelQueueInputFunction)(_mainDelQue.argc[i].argv);
        free(_mainDelQue.argc[i].argv);
    }
}