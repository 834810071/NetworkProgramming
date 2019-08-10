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


void echosrv(int clientfd)
{
    char recvbuf[1024];
    while (1) {
        memset(recvbuf, 0, sizeof recvbuf);
        int ret;
        if ((ret = read(clientfd, recvbuf, sizeof(recvbuf))) < 0) {
            ERR_EXIT("read");
        }
        if (ret == 0) {
            printf("client close\n");
            break;
        }
        fputs(recvbuf, stdout);
        write(clientfd, &recvbuf, strlen(recvbuf));
    }
    close(clientfd);
}

int main(int argc, char** argv)
{
    int listenfd;

    unlink("/home/jxq/CLionProjects/NetworkProgramming/text_unix");    // 解除原有text_unix对象链接
    if ((listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        ERR_EXIT("socket");
    }

    struct sockaddr_un servaddr;
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, "/home/jxq/CLionProjects/NetworkProgramming/text_unix");

    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof servaddr) < 0)
    {
        ERR_EXIT("bind");
    }

    if (listen(listenfd, SOMAXCONN) < 0)
    {
        ERR_EXIT("listen");
    }

    struct sockaddr_un cliaddr;
    socklen_t clilen;
    int clientfd;
    while (1)
    {
        if ((clientfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0)
        {
            ERR_EXIT("accept");
        }
        echosrv(clientfd);
    }
    close(listenfd);

    return 0;
}