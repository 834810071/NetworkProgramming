//
// Created by jxq on 19-8-8.
//

// socket编程15 UDP聊天实验室实现

#include "Pub.h"
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

char username[16];
USER_LIST client_list;

#define ERR_EXIT(m) \
        do \
        {   \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while (0);


void do_someone_login(MESSAGE msg)
{
    puts("do_someone_login\n");
}

void do_someone_logout(MESSAGE msg)
{
    puts("do_someone_logout\n");
}

void do_getlist(int sock)
{
    puts("do_getlist\n");
}

void parse_cmd(char *cmdline, int sock,struct sockaddr_in* server_addr)
{
    cout << "parse_cmd" << endl;
}
void chat_cli(int sock)
{
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6666);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    MESSAGE msg;
    int ret;
    while (1)
    {
        memset(&msg, 0, sizeof(msg));
        cout << "please input your name:";
        fflush(stdout);
        scanf("%s", username);

        memset(&msg, 0, sizeof(msg));
        msg.cmd = ntohs(C2S_LOGIN);
        strcpy(msg.body, username);

        sendto(sock, &msg, sizeof(msg), 0, (struct sockaddr *)&server_addr, sizeof server_addr);

        memset(&msg, 0, sizeof(msg));
        ret = recvfrom(sock, &msg, sizeof(msg), 0, NULL, NULL);
        if (ret == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            ERR_EXIT("recvfrom")
        }

        int cmd = ntohs(msg.cmd);
        if (cmd == S2C_ALREADY_LOGINED)
        {
            printf("user %s already logined server, please use another user\n", msg.body);
        }
        else if (cmd == S2C_LOGIN_OK)
        {
            printf("user %s has logined server\n", username);
            break;
        }
    }

    int count = 0;
    recvfrom(sock, &count, sizeof(int), 0, NULL, NULL);

    int n = ntohs(count);
    printf("has %d users logined server\n", n);

    for (int i = 0; i < n; ++i)
    {
        USER_INFO user;
        recvfrom(sock, &user, sizeof(USER_INFO), 0, NULL, NULL);
        client_list.push_back(user);
        in_addr tmp;
        tmp.s_addr = user.ip;

        printf("%d %s <-> %s:%d\n", i, user.username, inet_ntoa(tmp), user.port);
    }
    printf("\n Commands are:\n");
    printf("send username msg\n");
    printf("list\n");
    printf("exit\n");
    printf("\n");

    fd_set rset;
    FD_ZERO(&rset);
    int nready;
    socklen_t peerlen;
    struct sockaddr_in peeraddr;
    while (1)
    {
        FD_SET(STDIN_FILENO, &rset);
        FD_SET(sock, &rset);
        nready = select(sock+1, &rset, NULL, NULL, NULL);
        if (nready == -1)
        {
            ERR_EXIT("select");
        }
        if (nready == 0)
        {
            continue;
        }
        if (FD_ISSET(sock, &rset))
        {
            peerlen = sizeof(peeraddr);
            memset(&msg, 0, sizeof(msg));
            recvfrom(sock, &msg, sizeof(msg), 0, (struct sockaddr *)&peeraddr, &peerlen);
            int cmd = ntohs(msg.cmd);
            switch (cmd)
            {
                case S2C_SOMEONE_LOGIN:
                    do_someone_login(msg);
                    break;
                case S2C_SOMEONE_LOGOUT:
                    do_someone_logout(msg);
                    break;
                case S2C_ONLINE_USER:
                    do_getlist(sock);
                    break;
                default:
                    break;
            }
        }
        if (FD_ISSET(STDIN_FILENO, &rset))
        {
            char cmdline[100] = {0};
            if (fgets(cmdline, sizeof(cmdline), stdin) == NULL)
            {
                break;
            }
            if (cmdline[0] == '\n')
            {
                continue;
            }
            cmdline[strlen(cmdline) - 1] = '\0';
            parse_cmd(cmdline, sock, &server_addr);
        };
    }
}

int main(void)
{
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        ERR_EXIT("socket");
    }
    chat_cli(sock);

    return 0;
}
