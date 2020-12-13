#include "net_packet.h"
#include <string.h>

NetPacket::NetPacket(char *buffer, int size)
{
    index = 0;
    this->buffer = buffer;
    this->size = size;
}

int NetPacket::Length()
{
    return index;
}

bool NetPacket::CheckIsAndSkip(const char *str)
{
    int i = 0;
    int len = index >= size ? 0 : (unsigned char)buffer[index];
    while (i < len && buffer[i + index + 1] == str[i] && str[i] != 0)
    {
        i++;
    }
    bool b = i == len && str[i] == 0;
    if (b) index += (i + 1);
    return b;
}

void NetPacket::PackChar(unsigned char c)
{
    if (index + 1 > size) return;
    buffer[index++] = c;
}

void NetPacket::PackInt(unsigned int i)
{
    PackChar((unsigned char)(i & 0xff));
    i = i >> 8;
    PackChar((unsigned char)(i & 0xff));
    i = i >> 8;
    PackChar((unsigned char)(i & 0xff));
    i = i >> 8;
    PackChar((unsigned char)(i & 0xff));
}

void NetPacket::PackString(const char *src)
{
    int len = strlen(src);
    if (len > 255) len = 255;
    if (index + len > size) return;
    PackChar((unsigned char)len);
    memcpy(buffer + index, src, len);
    index += len;
}

unsigned char NetPacket::UnpackChar()
{
    if (index + 1 > size) return 0;
    return (unsigned char)buffer[index++];
}

unsigned int NetPacket::UnpackInt()
{
    unsigned int i = 0;
    i = i | (unsigned int) UnpackChar();
    i = i | ((unsigned int) UnpackChar() << 8);
    i = i | ((unsigned int) UnpackChar() << 16);
    i = i | ((unsigned int) UnpackChar() << 24);
    return i;
}

void NetPacket::UnpackString(char *dst)
{
    int len = UnpackChar();
    if (index + len > size) len = size - index;
    if (len != 0)
    {
        memcpy(dst, buffer + index, len);
    }
    dst[len] = 0;
    index += len;
}