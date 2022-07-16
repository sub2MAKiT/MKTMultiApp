#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE*MKTFILE = fopen("./build/files/files.MKTI","wb");
    if(MKTFILE!=NULL)
    {
        char buffer[7] = {'M','K','T','P',' ',1,'\n'};
        fwrite(buffer,1,7,MKTFILE);
    }
    return 0;
}