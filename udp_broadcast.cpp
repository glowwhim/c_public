#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "udp_broadcast.h"


UdpBroadcast::UdpBroadcast()
{
    socket_fd = -1;
}

int UdpBroadcast::Start() 
{
    int time_live = 64;
    socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (socket_fd == -1) return 1;

    if (setsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&time_live, sizeof(time_live)) == -1) return 2;

    return 0;
}

void UdpBroadcast::Send(char *group, int port, char *data, int len) 
{
    struct sockaddr_in mul_adr;
    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(group);
    mul_adr.sin_port = htons(port);

    sendto(socket_fd, data, len, 0, (struct sockaddr *)&mul_adr, sizeof(mul_adr));
}

void UdpBroadcast::Close()
{
    close(socket_fd);
    socket_fd = -1;
}