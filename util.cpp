#include <stdio.h>

namespace c_public_util
{

    void PrintByteCode(const char *bytes, int len)
    {
        for (int i = 0; i < len; i++)
        {
            printf("%02X-%c ", bytes[i], bytes[i]); 
        }
        printf("\n");
    }

}