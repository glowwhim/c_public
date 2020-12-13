class NetPacket
{

    private:
    int index;
    char *buffer;
    int size;

    public:
    NetPacket(char *buffer, int size);
    int Length();
    void PackChar(unsigned char c);
    void PackInt(unsigned int i);
    void PackString(const char *src);

    unsigned char UnpackChar();
    unsigned int UnpackInt();
    void UnpackString(char *dst);

};