#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp_server.h"

TcpServer::TcpServer()
{
    on_receive_data = nullptr;
}

void TcpServer::OnReceiveData(int connect_fd, char *data)
{

}

void TcpServer::Send(int connect_fd, char *data, int len)
{
    send(connect_fd, data, len, 0);
}

int TcpServer::Run(int port)
{
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) return 1;
    printf("create socket: %d\n", socket_fd);

    int on;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) == -1) return 2;
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) return 3;
    printf("socket bind success\n");

    if (listen(socket_fd, 512) == -1) return 4;
    printf("socket listen success\n");

    fd_set fdsr;
    int max_fd = socket_fd;
    while (true) max_fd = CheckReceive(&fdsr, max_fd);

    for (int i = 0; i < connect_list.size(); i++)
        close(connect_list[i]);
    return 0;
}

void TcpServer::CloseConnection(int connect_index, fd_set *fdsr)
{
    int connect_fd = connect_list[connect_index];
    close(connect_fd);
    FD_CLR(connect_fd, fdsr);
    connect_list.erase(connect_list.begin() + connect_index);
    printf("close connection: %d\n", connect_fd);
}

int TcpServer::CheckReceive(fd_set *fdsr, int max_fd)
{
        // initialize file descriptor set
        FD_ZERO(fdsr);
        FD_SET(socket_fd, fdsr);
        for (int i = 0; i < connect_list.size(); i++)
            FD_SET(connect_list[i], fdsr);

        // timeout setting
        struct timeval tv;
        tv.tv_sec = 40;
        tv.tv_usec = 0;

        int ret = select(max_fd + 1, fdsr, NULL, NULL, &tv);
        if (ret <= 0) return max_fd;

        // check connection receive data
        for (int i = 0; i < connect_list.size(); i++)
        {
            int connect_fd = connect_list[i];
            if (!FD_ISSET(connect_fd, fdsr)) continue;
            ret = recv(connect_fd, buffer, TCP_SERVER_BUFFER_SIZE, 0);
            if (ret <= 0) {
                CloseConnection(i--, fdsr);
                printf("close connection: %d\n", connect_fd);
            } else {
                if (ret >= TCP_SERVER_BUFFER_SIZE) ret = TCP_SERVER_BUFFER_SIZE - 1;
                buffer[ret] = '\0';
                printf("====================receive data====================\n");
                printf("%s\n", buffer);
                // handle receive
                OnReceiveData(connect_fd, buffer);
                if (on_receive_data != nullptr) on_receive_data(connect_fd, buffer);
                if (!keepConnect) CloseConnection(i--, fdsr);
            }
        }

        // check new connection
        if (FD_ISSET(socket_fd, fdsr)) {
            struct sockaddr_in client_addr;
            socklen_t sin_size = sizeof(client_addr);
            int new_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &sin_size);
            if (new_fd <= 0) return max_fd;
            printf("new connection: %d\n", new_fd);
            connect_list.push_back(new_fd);
            if (new_fd > max_fd) max_fd = new_fd;
        }
        return max_fd;
}

void HttpServer::OnReceiveData(int connect_fd, char *data)
{
    char content_type[64];
    char content[4096];
    char header[1024];
    GetContent(content_type, content);
    sprintf(header, HTTP_HEADER, strlen(content), content_type);
    send(connect_fd, header, strlen(header), 0);
    send(connect_fd, content, strlen(content), 0);
}

void HttpServer::GetContent(char *type, char *content)
{
    strcpy(type, "text");
    strcpy(content, "hello");
}