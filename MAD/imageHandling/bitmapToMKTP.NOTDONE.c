#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE* MKTFILE = fopen("./logo.pbm","rb");
    if(MKTFILE != NULL)
    {
        void *list;
        fseek(MKTFILE,0L,SEEK_END);
        long sizeOfFile = ftell(MKTFILE);
        rewind(MKTFILE);
        list = malloc(sizeOfFile);
        fread( list,1, sizeOfFile, MKTFILE );
        fclose( MKTFILE );
        size_t offset = 0;
        while(*((char*)list+offset) != '\n')
            offset++;
        offset++;
        int multiplierInSpace = 1;
        int width = 0;
        int height = 0;
        while(*((char*)list+offset) != '\n')
        {
            width *= 10;
            width += (*((char*)list+offset) - '0');
            offset++;
        }
        offset++;
        while(*((char*)list+offset) != '\n')
        {
            height *= 10;
            height += (*((char*)list+offset) - '0');
            offset++;
        }
        
    } else {
        printf("PROBLEMA CHIEF");
    }
    return 0;
}