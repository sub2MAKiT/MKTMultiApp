typedef struct MKTFILEINFO{
    void * data;
    unsigned long long int sizeOfData;
    int ID;
    char type;
} MKTInfo;

typedef struct initInput {
    int windowX;
    int windowY;
} initi;

typedef struct functionInput {
    int(*loadFile)(const char * FP,char mode);
    void*(*MKTUSE)(MKTInfo data);
    double mouseX;
    double mouseY;
    int buttonPressed;
    void * otherData;
    char cmdMode;
    int windowX;
    int windowY;
} funi;

typedef struct cleanUpInput {
    long int * errorCode;
} cleanUpi;