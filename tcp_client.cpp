#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <pthread.h>
#include "tcp_client.h"
#include "util.cpp"

using c_public_util::PrintByteCode;

TcpClient::TcpClient()
{
    socket_fd = -1;
}

int TcpClient::Connect(const char *ip, int port)
{
    if (socket_fd != -1) return 0;

    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return 1;
    printf("create socket success\n");

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if( inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0) return 2;

    if (connect(socket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) return 3;
    printf("connect success\n");

    return 0;
}

int TcpClient::StartListen()
{
    if (socket_fd == -1) return 1;

    pthread_t tids[1];
    pthread_create(tids, NULL, ListenThread, this);

    return 0;
}

void* TcpClient::ListenThread(void *arg)
{
    printf("start listen\n");
    TcpClient *client = (TcpClient*) arg;
    char buffer[TCP_CLIENT_RECEIVE_BUFFER_SIZE];
    while (1)
    {
        int n = recv(client->socket_fd, buffer, TCP_CLIENT_RECEIVE_BUFFER_SIZE - 1, 0);
        if (n <= 0) break;
        buffer[n] = '\0';
        printf("====================receive data====================\n");
        printf("%s\n", buffer);
    }
    printf("stop listen\n");
    client->Close();
    return NULL;
}

void TcpClient::Send(const char *data, int len)
{
    send(socket_fd, data, len, 0);
    
    char buffer[TCP_CLIENT_RECEIVE_BUFFER_SIZE];
    if (len >= TCP_CLIENT_RECEIVE_BUFFER_SIZE) len = TCP_CLIENT_RECEIVE_BUFFER_SIZE - 1;
    memcpy(buffer, data, len);
    buffer[len] = 0;
    PrintByteCode(data, len);
}

void TcpClient::Close()
{
    if (socket_fd == -1) return;
    int to_close = socket_fd;
    socket_fd = -1;
    close(to_close);
    printf("close socket\n");
}