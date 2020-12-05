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
    virtual void OnReceiveData(int connect_fd, char *data);

    public:
    void (*on_receive_data)(int connect_fd, char *data);

    TcpServer();
    int Run(int port);

};

class HttpServer : public TcpServer {

    protected:
    void OnReceiveData(int connect_fd, char *data);
    virtual void GetContent(char *type, char *content);

};