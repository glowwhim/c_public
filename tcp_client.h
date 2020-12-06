

#define TCP_CLIENT_RECEIVE_BUFFER_SIZE 4096

class TcpClient {

    private:
    int socket_fd;
    static void* ListenThread(void *arg);

    public:
    TcpClient();
    int Connect(const char *ip, int port);
    int StartListen();
    void Send(const char *data, int len);
    void Close();

};