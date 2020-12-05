

class UdpBroadcast 
{

    private:
    int socket_fd;

    public:
    UdpBroadcast();
    int Start();
    void Send(char *group, int port, char *data, int len);
    void Close();

};