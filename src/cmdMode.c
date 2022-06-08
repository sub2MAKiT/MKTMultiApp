#define cmdModeDirectInclude
#include "cmdMode.h"

const char _MKTMA_VERSION[9] = "00.00.01";

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

void  MKTMAcmdMode()
{
    char * command = (char*)malloc(100); // default command buffer
    printf("\n WELCOME TO MKTMULTIAPP CMD MODE:\n");
    do {
        for(int i = 0; i < 100; i++)
            command[i] = 0;
        printf("\n\033[31;40mCMD_USER\033[93;40m@\033[92;40mMultiApp\033[97;40m:\033[94;40m~\033[92;40m$ \033[97;40m");
        scanf("%99s",command);

        if( MKTcompStr(command,"help"))
            _MKTMACMD_help();
        else if( MKTcompStr(command,"version"))
            _MKTMACMD_version();

    } while(!(MKTcompStr(command,"exit")));
    return;
}

char MKTcompStr(char * charArray, const char * string)
{
    char checkForEnd = 0;
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