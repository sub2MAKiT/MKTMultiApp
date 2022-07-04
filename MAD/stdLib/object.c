char MKTcompStr(char * charArray, const char * string)
{
    char checkForEnd = 0;
    for(int i = 0; i < 100 && string[i] != 0; i++)
        if(string[i] != charArray[i])
            return 0;

    return 1;
}