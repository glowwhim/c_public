#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

namespace net_utility
{

    int UdpSend(char *ip, int port, char *data, int len)
    {
        int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(socket_fd < 0) return 1;

        struct sockaddr_in addr_serv;
        memset(&addr_serv, 0, sizeof(addr_serv));
        addr_serv.sin_family = AF_INET;
        addr_serv.sin_addr.s_addr = inet_addr(ip);
        addr_serv.sin_port = htons(port);

        sendto(socket_fd, data, len, 0, (struct sockaddr *)&addr_serv, sizeof(addr_serv));

        close(socket_fd);
        return 0;
    }

    int UdpListen()
    {
        int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (socket_fd == -1) return 1;

        struct sockaddr_in addr_serv;
        memset(&addr_serv, 0, sizeof(struct sockaddr_in));
        addr_serv.sin_family = AF_INET;
        addr_serv.sin_port = htons(4001);
        addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);
        int len = sizeof(addr_serv);

        if(bind(socket_fd, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0) return 2;


        int recv_num;
        char recv_buf[64];
        struct sockaddr_in addr_client;

        while(1)
        {
            recv_num = recvfrom(socket_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);
            recv_buf[recv_num] = '\0';
            printf("%s\n", recv_buf);
        }

        close(socket_fd);
        return 0;
    }

    int Broadcast(char *group, int port) 
    {
        int time_live = 64;
        int socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
        if (socket_fd == -1) return 1;
        if (setsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&time_live, sizeof(time_live)) == -1) return 2;

        struct sockaddr_in mul_adr;
        memset(&mul_adr, 0, sizeof(mul_adr));
        mul_adr.sin_family = AF_INET;
        mul_adr.sin_addr.s_addr = inet_addr("234.2.2.2");
        mul_adr.sin_port = htons(4000);

        while (1)
        {
            char data[] = "broadcast";
            sendto(socket_fd, data, strlen(data), 0, (struct sockaddr *)&mul_adr, sizeof(mul_adr));
            sleep(1);
        }

        close(socket_fd);
        return 0;
    }

    int ListenBroadcast(char *group, int port)
    {
        int socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
        if (socket_fd == -1) return 1;

        sockaddr_in adr;
        memset(&adr, 0, sizeof(adr));
        adr.sin_family = AF_INET;
        adr.sin_addr.s_addr = htonl(INADDR_ANY);
        adr.sin_port = htons(port);

        if(bind(socket_fd, (struct sockaddr *)&adr, sizeof(adr)) == -1) return 2;

        struct ip_mreq join_adr;
        join_adr.imr_multiaddr.s_addr = inet_addr(group);
        join_adr.imr_interface.s_addr = htonl(INADDR_ANY);
        setsockopt(socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&join_adr, sizeof(join_adr));

        int BUFFER_SIZE = 64;
        char buffer[BUFFER_SIZE];
        while (1) {
            int len = recvfrom(socket_fd, buffer, BUFFER_SIZE - 1, 0, NULL, 0);
            if(len < 0) break;
            buffer[len] = 0;
            printf("%s\n", buffer);
        }
        close(socket_fd);
        return 0;
    }

}