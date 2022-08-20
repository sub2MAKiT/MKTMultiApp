typedef struct MKTFILECONVERTERSTRUCT {
size_t sizeOfString;
char * string;
char type;
void*(*loadFile)(char * FP,int * error);
} MKTfileImporter;

int loadFile(char * filePath, char mode);
MKTPic _MKT_loadImage(char * filePath,int * error);