#include "errorHandling.h"

void MKTsetupError()
{
    _errors = (long*)malloc(sizeof(long)*1);
    _sizeOfErrors = 1;
    _errors[0] = 0;
    return;
}

void MKTerror(IntDex errorCode)
{
    _sizeOfErrors++;
    _errors = (long*)realloc(_errors,sizeof(long)*_sizeOfErrors);
    _errors[_sizeOfErrors-1] = errorCode;
    printf("ERROR:%lld",errorCode);
    return;
}

void MKTreturnError(const char * FP)
{
    FILE * errorFile = fopen(FP,"wb");

    for(IntDex i = 0; i < _sizeOfErrors;i++)
    {
        char buffer[100];
        IntDex toWrite = snprintf(buffer,100,"%ld",_errors[i]);
        fwrite(buffer,toWrite,1,errorFile);
    }

    free(_errors);
    fclose(errorFile);
    return;
}