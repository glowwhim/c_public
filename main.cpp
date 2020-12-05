#include "tcp_server.h"
#include <netinet/in.h>

// g++ main.cpp tcp_server.cpp -o main.out

void on_receive(int connect_fd, char *data)
{
    //send(connect_fd, "hello", 5, 0);
}

int main(int argc, char* argv[])
{
    HttpServer server;
    server.on_receive_data = on_receive;
    server.Run(4000);
    return 0;
}