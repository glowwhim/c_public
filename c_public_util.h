#pragma once

class CPublicUtil
{

    public:
    static void PrintByteCode(const char *bytes, int len);
    static void AppendFile(const char *path, const char *data, int len);
    static void WriteFile(const char *path, const char *data, int len);

};