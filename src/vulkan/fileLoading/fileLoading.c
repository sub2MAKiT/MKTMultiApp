#include <MKTAppEngine.h>
#include <fileLoading/fileLoading.h>
#include <../MKTDLL/MKTDLL.h>

IntDex loadFile(char * FP)
{

    MKTInfo * tempData;
    unsigned long long int tempSize = 0;
    for(int i = 0; i < _MKT_sizeOfFileModules;i++)
        if(MKTstrcmp(MKTdissectFileType(&tempSize, FP), _MKT_fileModules[i].FileName))
            tempData = (*_MKT_fileModules[i].load)(FP);

    
    char type = tempData->type;
    
    printf("test %d\n",tempData->type);


    IntDex returnValue = 0;

    if(type == MKTINFOV) // 0
        0;
    else if(type == MKTAGV) // 1
        _MKT_genAG(tempData->data);
    else if(type == MKTAUDIOV) // 2
        _MKT_genPiC(tempData->data);
    else if(type == MKTPICV) // 3
        0;
    else if(type == MKTVIDV) // 4
        0;
    else if(type == MKTBMV) // 5
        _MKT_genBM(tempData->data);
    else if(type == MKTFONTMV) // 6
        0;
    else if(type == MKTOBJV) // 7
        0;
    else
        0;

    free(tempData->data);
    free(tempData);

    return returnValue;
}