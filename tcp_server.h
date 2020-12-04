#include <vector>
#include <sys/types.h>

#define TCP_SERVER_BUFFER_SIZE 4096

class TcpServer {

    private:
    int socket_fd;
    std::vector<int> connect_list;
    char buffer[TCP_SERVER_BUFFER_SIZE];
    int CheckReceive(fd_set *fdsr, int max_fd);

    public:
    void (*on_receive_data)(int connect_fd, char *data);

    TcpServer();
    int Run(int port);

};