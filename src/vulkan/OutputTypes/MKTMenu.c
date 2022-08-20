void loadMenuAG()
{
    const char fileNames[19] = "./lib/icons/.MKTAG";

    CBT = 0.0;

    FILE *MKTFILE;
    MKTFILE = fopen( "./lib/libraryList.MKTI", "rb" );
    char *charArray;
    long sizeOfFile;
    if( MKTFILE != NULL )
    {
        fseek(MKTFILE, 0L, SEEK_END);
        sizeOfFile = ftell(MKTFILE);
        rewind(MKTFILE);
        charArray = (char*)malloc(sizeOfFile);
        sizeOfFile = fread( charArray,1, sizeOfFile, MKTFILE );
        fclose( MKTFILE );
    } else {
        // error file couldn't openprintf
    }
    int moduleNumber = 0;
    for(int i = 1; i < sizeOfFile;i++)
    {
        if(charArray[i-1] == '\n' && charArray[i] != '{')
        {
            char * MKTAGName;
            size_t sizeOfName = 0;
        for(0; charArray[i+sizeOfName+1] != '\n' && i+sizeOfName < sizeOfFile;sizeOfName++)0;
            MKTAGName = (char*)malloc(sizeOfName+19);
            for(int a = 0; a < sizeOfName;a++)
                MKTAGName[a+12] = charArray[a+i];
            for(int a = 0; a < 12;a++)
                MKTAGName[a] = fileNames[a];
            for(int a = 0; a < 7;a++)
                MKTAGName[sizeOfName+12+a] = fileNames[a+12];
            Modules[moduleNumber].icon.AG = arrayGraphicsReader(MKTAGName);
            AGPushConstants DEFAULTconstants;
            DEFAULTconstants.colourModification = {1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f}; // black
            float ratio = _windowExtent.width;
            ratio /= _windowExtent.height;
            DEFAULTconstants.transformation = {0.05/ratio,0.0,0.0,0.0,0.0,0.05,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

            DEFAULTconstants.movement = {0.0-((_windowExtent.width*0.98)/_windowExtent.width),0.0-((_windowExtent.height*0.95)/_windowExtent.height),0.0};
            Modules[moduleNumber].icon.isVisible = 1;
            Modules[moduleNumber].icon.AGPC = DEFAULTconstants;
            upload_AG(Modules[moduleNumber].icon.AG);
            moduleNumber++;
            free(MKTAGName);
        }
    }
    if(moduleNumber != sizeOfModules)
        0;
}