#include <vector>
#include <sys/types.h>

#define TCP_SERVER_BUFFER_SIZE 4096

const char HTTP_HEADER[] = "HTTP/1.1 200 OK\n"
    "Content-Length: %ld\nConnection: close\n"
    "Content-Type: %s\n\n";

class TcpServer {

    private:
    std::vector<int> connect_list;
    char buffer[TCP_SERVER_BUFFER_SIZE];
    int CheckReceive(fd_set *fdsr, int max_fd);
    void CloseConnection(int connect_index, fd_set *fdsr);

    protected:
    bool keepConnect;
    int socket_fd;
    virtual void OnReceiveData(int connect_fd, char *data, int size);

    public:
    void (*on_receive_data)(int connect_fd, char *data, int size);

    TcpServer();
    int Run(int port);
    void Send(int connect_fd, char *data, int len);

};

class HttpServer : public TcpServer {

    protected:
    void OnReceiveData(int connect_fd, char *data, int size);
    virtual void GetContent(char *type, char *content);

};