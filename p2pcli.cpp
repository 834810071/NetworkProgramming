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

void handler(int sig)
{
    printf("recv a sig = %d\n", sig);
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
    // 1. 创建套接字
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        ERR_EXIT("socket");
    }

    // 2. 分配套接字地址
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);
    // servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // inet_aton("127.0.0.1", &servaddr.sin_addr);

    // 3. 请求链接
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof servaddr) < 0) {
        ERR_EXIT("connect");
    }

    // 4. 数据交换
    char recvbuf[1024];
    char sendbuf[1024];
    pid_t pid;
    pid = fork();
    if (pid == -1)
    {
        ERR_EXIT("fork");
    }
    if (pid == 0)
    {

        while (1)
        {
            int ret = read(sockfd, recvbuf, sizeof recvbuf);    // 服务器读取
            if (ret == 0)
            {
                printf("server close\n");
                break;
            } else if (ret == -1)
            {
                ERR_EXIT("read");
            }
            fputs(recvbuf, stdout); // 服务器返回数据输出
        }
        printf("child exit\n");
        kill(getppid(), SIGUSR1);
        //exit(EXIT_SUCCESS);
        close(sockfd);
    } else{
        signal(SIGUSR1, handler);
        while (fgets(sendbuf, sizeof sendbuf, stdin) != NULL)   // 键盘输入获取
        {
            write(sockfd, sendbuf, sizeof sendbuf); // 写入服务器
            // 清空
            memset(sendbuf, 0, sizeof sendbuf);
        }
        printf("parent exit\n");
       // exit(EXIT_SUCCESS);
        close(sockfd);

    }

    // 5. 断开连接
    close(sockfd);



    return 0;
}
