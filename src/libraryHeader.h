typedef struct AGDATAFORRENDER {
    double movement[3];
    double transformation[16];
    int ID;
} AGDFR;
typedef struct functionInput {
    int(*loadAG)(const char * FP);
    void(*displayAG)(AGDFR data);
    double mouseX;
    double mouseY;
    int buttonPressed;
    void * otherData;
    char cmdMode;
} funi;