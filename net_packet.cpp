#include "net_packet.h"
#include <string.h>

NetPacket::NetPacket(char *buffer)
{
    index = 0;
    this->buffer = buffer;
}

void NetPacket::PackChar(unsigned char c)
{
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
    PackChar((unsigned char)len);
    memcpy(buffer + index, src, len);
    index += len;
}

unsigned char NetPacket::UnpackChar()
{
    return (unsigned char)buffer[index++];
}

unsigned int NetPacket::UnpackInt()
{
    unsigned int i = 0;
    i = i | (unsigned int) UnpackChar();
    i = i | (unsigned int) UnpackChar() << 8;
    i = i | (unsigned int) UnpackChar() << 16;
    i = i | (unsigned int) UnpackChar() << 24;
    return i;
}

void NetPacket::UnpackString(char *dst)
{
    int len = UnpackChar();
    if (len != 0)
    {
        memcpy(dst, buffer + index, len);
    }
    dst[len] = 0;
    index += len;
}