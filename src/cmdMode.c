#define cmdModeDirectInclude
#include "cmdMode.h"

const char _MKTMA_VERSION[9] = "00.04.00";

void handleCmdGrapics(char mode)
{
    FILE *MKTDATA = fopen("currentMode", "wb" );       
    
    fwrite(&mode,1,1,MKTDATA);

    fclose( MKTDATA );

    return;
}

char checkForCmdMode(int argc, char ** argv)
{
    if(argc != 1)
    {
        if(argv[1][0] == 'h')
        {
            printf("\nHelp option coming soon\n");
            return 2;
        } else if(argv[1][0] == 'c' && argv[1][1] == 'm' && argv[1][2] == 'd')
            return 1;
    }
    return 0;
}

void MKTMAcmdMode(void (**funArray)(void*), long sizeOfFunArray)
{
    char * command = (char*)malloc(100); // default command buffer
    printf("\nWELCOME TO MKTMULTIAPP CMD MODE:\n");
    do {
        for(int i = 0; i < 100; i++)
            command[i] = 0;
        printf("\n\033[31;40mCMD_USER\033[93;40m@\033[92;40mMultiApp\033[97;40m:\033[94;40m~\033[92;40m$ \033[97;40m");
        scanf("%99s",command);

        if( MKTcompStr(command,"help"))
            _MKTMACMD_help();
        else if( MKTcompStr(command,"version"))
            _MKTMACMD_version();
        else if(MKTcompStr(command, "cp"))
            _MKTMACMD_copy(command);
        else {
            for(int i = 0; i < sizeOfFunArray;i++)
                funArray[i]((void*)command);
        }

    } while(!(MKTcompStr(command,"exit")));
    return;
}

void _MKTMACMD_copy(char * command)
{
    if(!(command[3] == 0 || command[4] == 0 || command[5] == 0))
    {
        char * src = (char*)malloc(1);
        char * dst = (char*)malloc(1);
        int startingPoint;
        for(int i = 3;command[i]!='|';i++)
        {
            src = (char*)realloc(src,i-1);
            src[i-3] = command[i];
            src[i-2] = 0;
            startingPoint = i+1;
            printf("\nA: %d",i);
        }
        for(int i = startingPoint+1;command[i]!=0;i++)
        {
            dst = (char*)realloc(dst,i-startingPoint+1);
            dst[i-startingPoint-1] = command[i];
            dst[i-startingPoint] = 0;
            printf("\nB: %d",i);
        }
        FILE *SRCFILE;
        FILE *DSTFILE;
        SRCFILE = fopen(src, "rb" ); // ./graphics/cmdMode = comedy
        DSTFILE = fopen(dst,"wb");
        printf("\nC: %d ",startingPoint);
        for(int i = 0; i < 10;i++)
            printf("%c",src[i]);
        printf("\nD: %d ",startingPoint);
        for(int i = 0; i < 10;i++)
            printf("%c",dst[i]);
        if( SRCFILE != NULL && DSTFILE != NULL)
        {
            char *charArray;
            fseek(SRCFILE, 0L, SEEK_END);
            long sizeOfFile = ftell(SRCFILE);
            rewind(SRCFILE);
            charArray = (char*)malloc(sizeOfFile);
            sizeOfFile = fread( charArray,1, sizeOfFile, SRCFILE );
            fwrite(charArray,1,sizeOfFile,DSTFILE);
            fclose( SRCFILE );
            fclose( DSTFILE );
        }
    } else {
        printf("\nERROR");
    }
    return;
}

char MKTcompStr(char * charArray, const char * string)
{
    for(int i = 0; i < 100 && string[i] != 0; i++)
        if(string[i] != charArray[i])
            return 0;

    return 1;
}

void _MKTMACMD_help()
{
    FILE *MKTFILE;
    MKTFILE = fopen( "./graphics/cmdMode", "rb" ); // ./graphics/cmdMode = comedy
    if( MKTFILE != NULL )
    {
        char *charArray;
        fseek(MKTFILE, 0L, SEEK_END);
        long sizeOfFile = ftell(MKTFILE);
        rewind(MKTFILE);
        charArray = (char*)malloc(sizeOfFile);
        sizeOfFile = fread( charArray,1, sizeOfFile, MKTFILE );
        fclose( MKTFILE );
        printf("\n");
        for(int i = 0; i < sizeOfFile; i++)
        {
            printf("%c",charArray[i]);
        }
    } else {
        printf("\nCannot find help file");
    }
    return;
}

void _MKTMACMD_version()
{
    printf("\nMKT MultiApp engine Version - ");
    for(int i = 0; i < 9;i++)
        printf("%c",_MKTMA_VERSION[i]);
    return;
}