#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include "tcp_client.h"

int TcpClient::Run(char *ip, int port)
{
    int socket_fd;
    char receiveBuffer[TCP_CLIENT_BUFFER_SIZE], sendBuffer[TCP_CLIENT_BUFFER_SIZE];
    struct sockaddr_in serverAddr;

    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return 1;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if( inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0) return 2;

    if (connect(socket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) return 3;
    printf("connect success\n");

    while (1)
    {
        printf("send msg to server: \n");
        fgets(sendBuffer, TCP_CLIENT_BUFFER_SIZE, stdin);
        if(send(socket_fd, sendBuffer, TCP_CLIENT_BUFFER_SIZE, 0) < 0) return 4;
        int n = recv(socket_fd, receiveBuffer, TCP_CLIENT_BUFFER_SIZE, 0);
        receiveBuffer[n] = '\0';
        printf("====================receive data====================\n");
        printf("%s\n", receiveBuffer);
    }
    close(socket_fd);
    return 0;
}