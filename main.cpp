#include "tcp_server.h"
#include <netinet/in.h>

void on_receive(int connect_fd, char *data)
{
    send(connect_fd, "hi", 3, 0);
}

int main(int argc, char* argv[])
{
    printf("hello");
    TcpServer server;
    server.on_receive_data = on_receive;
    server.Run(4000);
    return 0;
}