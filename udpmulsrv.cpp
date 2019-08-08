//
// Created by jxq on 19-8-8.
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define _SHORT_PORT    (8088)

//
// udp server heoo
//

void test(int sock, struct sockaddr_in peeraddr, char* recvbuf, int len, int al)
{
    //sendto(sock, "server", sizeof("server"), 0, (struct sockaddr *)&peeraddr, sizeof peeraddr);
    //sendto(sock, recvbuf, n, 0, (struct sockaddr *)&peeraddr, sizeof peeraddr);
    sendto(sock, recvbuf, len, 0, (struct sockaddr *)&peeraddr, al);
}

int main(int argc, char * argv[]) {
    int fd, len;
    struct sockaddr_in ar = { AF_INET };
    socklen_t al = sizeof (struct sockaddr_in);
    char msg[BUFSIZ];

    if ((fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("main socket dgram");
        exit(EXIT_FAILURE);
    }

    printf("udp server start [%d][0.0.0.0][%hd] ...\n", fd, _SHORT_PORT);

    // 服务器绑定地址
    ar.sin_port = htons(_SHORT_PORT);
    if (bind(fd, (struct sockaddr *)&ar, al) == -1) {
        perror("main bind INADDR_ANY");
        exit(EXIT_FAILURE);
    }

    // 阻塞的接收不同客户端数据来回搞
    while ((len = recvfrom(fd, msg, sizeof msg - 1, 0, (struct sockaddr *)&ar, &al)) > 0) {
        msg[len] = '\0';
        printf("[%s:%hd] -> %s\n", inet_ntoa(ar.sin_addr), ntohs(ar.sin_port), msg);

        // 回显继续发送给客户端
        // sendto(fd, msg, len, 0, (struct sockaddr *)&ar, al);
        test(fd, ar, msg, len, al);
    }

    return close(fd);
}