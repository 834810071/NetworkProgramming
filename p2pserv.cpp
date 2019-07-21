//
// Created by jxq on 19-7-17.
//
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>


using namespace std;

#define ERR_EXIT(m) \
        do  \
        {   \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while(0);

void do_service(int connfd)
{
    char recvbuf[1024];
    while (1)
    {
        memset(recvbuf, 0, sizeof recvbuf);
        int ret = read(connfd, recvbuf, sizeof recvbuf);
        if (ret == 0)
        {
            printf("client close\n");
            break;
        } else if (ret == -1)
        {
            ERR_EXIT("read");
        }
        fputs(recvbuf, stdout);
        write(connfd, recvbuf, ret);
    }

}

void handler(int sig)
{
    printf("recv a sig = %d\n", sig);
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
    // 1. 创建套接字
    int listenfd;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        ERR_EXIT("socket");
    }

    // 2. 分配套接字地址
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // inet_aton("127.0.0.1", &servaddr.sin_addr);

    int on = 1;
    // 确保time_wait状态下同一端口仍可使用
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on) < 0)
    {
        ERR_EXIT("setsockopt");
    }

    // 3. 绑定套接字地址
    if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof servaddr) < 0) {
        ERR_EXIT("bind");
    }
    // 4. 等待连接请求状态
    if (listen(listenfd, SOMAXCONN) < 0) {
        ERR_EXIT("listen");
    }
    // 5. 允许连接
    struct sockaddr_in peeraddr;
    socklen_t peerlen = sizeof peeraddr;
    int connfd;
    if ((connfd = accept(listenfd, (struct sockaddr *) &peeraddr, &peerlen)) < 0) {
        ERR_EXIT("accept");
    }

    // 6. 数据交换
    pid_t pid;
    pid = fork();

    if (pid == -1)
    {
        ERR_EXIT("fork");
    }
    if (pid == 0)   // 子进程
    {
        signal(SIGUSR1, handler);
        char sendbuf[1024];
        while (fgets(sendbuf, sizeof sendbuf, stdin) != NULL)
        {
            write(connfd, sendbuf, sizeof sendbuf);
            memset(sendbuf, 0, sizeof(sendbuf));
        }
        printf("child exit\n");
        exit(EXIT_SUCCESS);
    }
    else
    {
        char recvbuf[1024];
        while (1)
        {
            memset(recvbuf, 0, sizeof recvbuf);
            int ret = read(connfd, recvbuf, sizeof recvbuf);
            if (ret == -1)
            {
                ERR_EXIT("read");
            }
            else if (ret == 0)
            {
                printf("peer close\n");
                break;
            }
            fputs(recvbuf, stdout);
        }
        printf("parent exit\n");
        kill(pid, SIGUSR1);
        exit(EXIT_SUCCESS);
    }

    // 7. 断开连接
    close(connfd);
    close(listenfd);



    return 0;
}
