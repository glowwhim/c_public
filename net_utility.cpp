#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

namespace net_utility
{

    void SendFile(int connect_fd, const char *path)
    {
        FILE *fp = fopen(path, "rb");
        char buffer[1024];
        bool eof = false;
        while (!eof)
        {
            int i;
            for (i = 0; i < 1024; i++)
            {
                int c = fgetc(fp);
                eof = c == EOF;
                if (eof) break;
                buffer[i] = (char) c;
            }
            if (i) send(connect_fd, buffer, i, 0);
        }
    }

    void SendHttpHeader(int connect_fd, const char *type)
    {
        const char HTTP_HEADER[] = "HTTP/1.1 200 OK\n"
        "Connection: close\n"
        "Content-Type: %s\n\n";
        char header[1024];
        sprintf(header, HTTP_HEADER, type);
        send(connect_fd, header, strlen(header), 0);
    }

    void SendHttpText(int connect_fd, const char *text, int len)
    {
        const char HTTP_HEADER[] = "HTTP/1.1 200 OK\n"
        "Content-Length: %d\nConnection: close\n"
        "Content-Type: text\n\n";
        char header[1024];
        sprintf(header, HTTP_HEADER, len);
        send(connect_fd, header, strlen(header), 0);
        send(connect_fd, text, len, 0);
    }

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

    int UdpListen(int port, void(*recv_callback)(char *data, int len))
    {
        int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (socket_fd == -1) return 1;

        struct sockaddr_in addr_serv;
        memset(&addr_serv, 0, sizeof(struct sockaddr_in));
        addr_serv.sin_family = AF_INET;
        addr_serv.sin_port = htons(port);
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
            recv_callback(recv_buf, recv_num);
        }

        close(socket_fd);
        return 0;
    }

    int ListenBroadcast(char *group, int port, void(*recv_callback)(char *data, int len))
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
            recv_callback(buffer, len);
        }
        close(socket_fd);
        return 0;
    }

}