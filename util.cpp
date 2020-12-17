#include <stdio.h>

namespace c_public_util
{

    void PrintByteCode(const char *bytes, int len)
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

}