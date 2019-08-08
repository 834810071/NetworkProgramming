//
// Created by jxq on 19-8-8.
//

// socket编程 13

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define ERR_EXIT(m) \
        do \
        {   \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while (0);

void test(int sock, struct sockaddr_in* peeraddr, char* recvbuf, int n)
{
    //sendto(sock, "server", sizeof("server"), 0, (struct sockaddr *)&peeraddr, sizeof peeraddr);
    sendto(sock, recvbuf, n, 0, (struct sockaddr *)&peeraddr, sizeof peeraddr);
}
void echo_srv(int sock)
{
    char recvbuf[1024] = {0};
    struct sockaddr_in peeraddr;
    socklen_t peerlen;
    int n;
    while (1)
    {
        peerlen = sizeof peeraddr;
        memset(recvbuf, 0, sizeof recvbuf);
        n = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&peeraddr, &peerlen);
        printf("id = %s, ", inet_ntoa(peeraddr.sin_addr));
        printf("port = %d\n", ntohs(peeraddr.sin_port));
        if (n == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            ERR_EXIT("recvfrom")
        }
        else if (n > 0)
        {
            fputs(recvbuf, stdout);
            //sendto(sock, recvbuf, n, 0, (struct sockaddr *)&peeraddr, peerlen);
            //sendto(sock, "h", sizeof("h"), 0, (struct sockaddr *)&peeraddr, sizeof peeraddr);
            test(sock, &peeraddr, recvbuf, n);
        }
    }
    close(sock);
};

int main(void)
{
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        ERR_EXIT("socket");
    }
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof servaddr);
    servaddr.sin_family = PF_INET;
    servaddr.sin_port = htons(6666);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&servaddr, sizeof servaddr) < 0)
    {
        ERR_EXIT("bind");
    }

    echo_srv(sock);

    return 0;
}