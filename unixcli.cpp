//
// Created by jxq on 19-8-10.
//

// socket编程 16

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

#define ERR_EXIT(m) \
        do \
        { \
            perror(m); \
            exit(EXIT_FAILURE); \
        } while(0);


void clisrv(int sock)
{
    char sendbuf[1024];
    char recvbuf[1024];
    memset(recvbuf, 0, sizeof(recvbuf));
    memset(sendbuf, 0, sizeof(sendbuf));
    while (fgets(sendbuf, sizeof sendbuf, stdin) != NULL)
    {
        if (write(sock, sendbuf, sizeof(sendbuf)) < 0)
        {
            ERR_EXIT("write");
        }
        int ret;
        if ((ret = read(sock, &recvbuf, sizeof recvbuf)) < 0)
        {
            ERR_EXIT("read");
        }
        if (ret == 0)
        {
            printf("server close\n");
            break;
        }
        fputs(recvbuf, stdout);
        memset(recvbuf, 0, sizeof(recvbuf));
        memset(sendbuf, 0, sizeof(sendbuf));
    }
}


int main(int argc, char** argv)
{
    int clientfd;
    if ((clientfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        ERR_EXIT("sokcet");
    }

    struct sockaddr_un cliaddr;
    cliaddr.sun_family = AF_UNIX;
    strcpy(cliaddr.sun_path, "/home/jxq/CLionProjects/NetworkProgramming/text_unix");

    int conn;
    if ((conn = connect(clientfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr))) < 0)
    {
        ERR_EXIT("connect");
    }

    clisrv(clientfd);


    return 0;
}