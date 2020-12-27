#include <iostream>
#include <string.h>
#include "c_public_util.h"

const char* CPublicUtil::get_file_name(const char *file_path)
{
    int i = strlen(file_path) - 1;
    while (i >= 0 && file_path[i] != '/') i--;
    return file_path + i + 1;
}

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

void CPublicUtil::WriteFile(const char *path, const char *data, int len)
{
    FILE *fp = NULL;
    fp = fopen(path, "wb");
    for (int i = 0; i < len; i++) fputc(data[i], fp);
    fclose(fp);
}
