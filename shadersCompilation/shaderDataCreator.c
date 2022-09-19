#include <stdio.h>
#include <stdlib.h>

int main(unsigned int argc, char ** argv)
{
    // sorry for hardcoding but I'm just speeding thru the creation of this tool
    FILE * MKTFILE = fopen(argv[1],"wb");

    char index[1];

    // printf("\nIs using indices (1 = yes, 0 = no): ");
    index[0] = (char)atoi(argv[2]);

    fwrite(index,1,1,MKTFILE);

    unsigned int Size[1];

    // printf("\nSize of push constants (0 if isn't using descriptors): ");
    Size[0] = (unsigned int)atoi(argv[3]);

    fwrite(Size,sizeof(unsigned int),1,MKTFILE);

    // printf("\nSize of descriptors (0 if isn't using descriptors): ");
    Size[0] = (unsigned int)atoi(argv[4]);

    fwrite(Size,sizeof(unsigned int),1,MKTFILE);
    
    return 0;
}