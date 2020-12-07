class NetPacket
{

    private:
    int index;
    char *buffer;

    public:
    NetPacket(char *buffer);
    void PackChar(unsigned char c);
    void PackInt(unsigned int i);
    void PackString(const char *src);

    unsigned char UnpackChar();
    unsigned int UnpackInt();
    void UnpackString(char *dst);

};