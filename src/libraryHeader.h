typedef struct AGDATAFORRENDER {
    double movement[3];
    double transformation[16];
} AGDFR;


typedef struct MKTFILEINFO{
    void * data;
    int ID;
    char type;
} MKTInfo;

typedef struct initInput {
    char userOS;
    int windowX;
    int windowY;
} initi;

typedef struct functionInput {
    int(*loadFile)(const char * FP,char mode);
    void(*MKTUSE)(MKTInfo data);
    double mouseX;
    double mouseY;
    int buttonPressed;
    void * otherData;
    char cmdMode;
} funi;

typedef struct cleanUpInput {
    int errorCode;
} cleanUpi;