#include <iostream>
#include "c_public_util.h"

void CPublicUtil::PrintByteCode(const char *bytes, int len)
{
    for (int i = 0; i < len; i++)
    {
        char c = bytes[i];
        bool printChar = c > 32 && c < 127;
        if (printChar) printf("%c ", c); 
        else printf("%02X ", (unsigned char) c); 
    }
    printf("\n");
}

void CPublicUtil::AppendFile(const char *path, const char *data, int len)
{
    FILE *fp = NULL;
    fp = fopen(path, "ab");
    for (int i = 0; i < len; i++) fputc(data[i], fp);
    fclose(fp);
}
