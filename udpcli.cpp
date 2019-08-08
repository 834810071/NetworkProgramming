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

using namespace std;

#define ERR_EXIT(m) \
        do \
        {   \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while (0);

int main(void)
{
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        ERR_EXIT("socket");
    }

    struct sockaddr_in server_addr;
    int srvlen;
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6666);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    struct sockaddr_in localaddr;
    socklen_t addrlen = sizeof localaddr;
    if (getsockname(sock, (struct sockaddr*)&localaddr, &addrlen) < 0)
    {
        ERR_EXIT("getsockname");
    }
    printf("id = %s, ", inet_ntoa(localaddr.sin_addr));
    printf("port = %d\n", ntohs(localaddr.sin_port));

    char sendbuf[1024];
    int ret;
    char recvbuf[1024];
    while (fgets(sendbuf, sizeof sendbuf, stdin) != NULL)
    {
        srvlen = sizeof server_addr;
        sendto(sock, sendbuf, sizeof(sendbuf), 0, (struct sockaddr *)&server_addr, srvlen);
        // cout << sendbuf << endl;
        ret = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
        if (ret == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            ERR_EXIT("recvfrom")
        }
        else if (ret > 0)
        {
            fputs(recvbuf, stdout);
            // sendto(sock, recvbuf, n, 0, (struct sockaddr *)&peeraddr, peerlen);
        }
        memset(sendbuf, 0, sizeof sendbuf);
        memset(recvbuf, 0, sizeof recvbuf);
    }

    return 0;
}