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

#define ERR_EXIT(m) \
        do \
        {   \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while (0);

// 聊天室成员列表
USER_LIST client_list;

void test(int sock, struct sockaddr_in* peeraddr)
{

    sendto(sock, "server", sizeof("server"), 0, (struct sockaddr *)&peeraddr, sizeof peeraddr);
}

void do_login(MESSAGE msg, int sock, struct sockaddr_in cliaddr)
{
    // cout << "do_login" << endl;
    USER_INFO user;
    strcpy(user.username, msg.body);
    user.ip = cliaddr.sin_addr.s_addr;
    user.port = cliaddr.sin_port;

    /* 查找用户 */
    USER_LIST::iterator it;
    for (it = client_list.begin(); it != client_list.end(); ++it)
    {
        if (strcmp(it->username, msg.body) == 0)
        {
            break;
        }
    }

    // 未找到用户
    if (it == client_list.end())
    {
        printf("has a user login: %s <-> %s:%d\n", msg.body, inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
        client_list.push_back(user);

        // 登录成功应答
        MESSAGE reply_msg;
        memset(&reply_msg, 0, sizeof(reply_msg));
        reply_msg.cmd = htons(S2C_LOGIN_OK);
        sendto(sock, &msg, sizeof(msg), 0, (struct sockaddr *)&cliaddr, sizeof cliaddr);

        int count = htons((int)client_list.size());
        // 发送在线人数
        sendto(sock, &count, sizeof(int), 0, (struct sockaddr*)&cliaddr, sizeof cliaddr);

        printf("sending user list information to: %s <-> %s:%d\n", msg.body, inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
        // 发送在线列表
        for (it = client_list.begin(); it != client_list.end(); ++it)
        {
            sendto(sock, &*it, sizeof(USER_INFO), 0, (struct sockaddr*)&cliaddr, sizeof cliaddr);
        }

        // 向其他用户通知有新用户登录
        for (it = client_list.begin(); it != client_list.end(); ++it)
        {
            if (strcmp(it->username, msg.body) == 0)    // 如果是当前用户，则跳过
            {
                continue;
            }

            struct sockaddr_in peeraddr;
            memset(&peeraddr, 0, sizeof peeraddr);
            peeraddr.sin_family = AF_INET;
            peeraddr.sin_port = it->port;
            peeraddr.sin_addr.s_addr = it->ip;

            msg.cmd = htons(S2C_SOMEONE_LOGIN);
            memcpy(msg.body, &user, sizeof(user));

            if (sendto(sock, &msg, sizeof(msg), 0, (struct sockaddr*)&peeraddr, sizeof peeraddr) < 0)
            {
                ERR_EXIT("sendto");
            }
        }

    }
    else    // 找到该用户  fixme[如果找到该用户之后还有其他操作吗?]
    {
        printf("user %s has already logined\n", msg.body);

        MESSAGE reply_msg;
        memset(&reply_msg, 0, sizeof(MESSAGE));
        reply_msg.cmd = htons(S2C_ALREADY_LOGINED);
       // int a = ntohs(reply_msg.cmd);
        sendto(sock, &reply_msg, sizeof(MESSAGE), 0, (struct sockaddr*)&cliaddr, sizeof cliaddr);
    }
}

void do_logout(MESSAGE msg, int sock, struct sockaddr_in cliaddr)
{
     //cout << "do_logout" << endl;
     printf("has a user logout : %s <-> %s:%d\n", msg.body, inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);

     USER_LIST::iterator it;
     for (it = client_list.begin(); it != client_list.end(); ++it)
     {
        if (strcmp(it->username, msg.body) == 0)
        {
            break;
        }
     }

     if (it != client_list.end())
     {
         client_list.erase(it);
     }

     // 向其他用户通知有用户退出
     for (it = client_list.begin(); it != client_list.end(); ++it)
     {
         if (strcmp(it->username, msg.body) == 0)
         {
             continue;
         }
         struct sockaddr_in peeraddr;
         memset(&peeraddr, 0, sizeof peeraddr);
         peeraddr.sin_family = AF_INET;
         peeraddr.sin_addr.s_addr = it->ip;
         peeraddr.sin_port = it->port;

         MESSAGE m;
         strcpy(m.body, msg.body);
         m.cmd = S2C_SOMEONE_LOGOUT;
         if (sendto(sock, &m, sizeof(MESSAGE), 0, (struct sockaddr*)&peeraddr, sizeof(peeraddr)) < 0)
         {
             ERR_EXIT("sendto");
         }
     }
}

void do_sendlist(int sock, struct sockaddr_in cliaddr)
{
    //cout << "do_sendlist" << endl;
    MESSAGE msg;
    msg.cmd = htons(S2C_ONLINE_USER);
    sendto(sock, (const char*)&msg, sizeof(msg), 0, (struct sockaddr*)&cliaddr, sizeof cliaddr);

    int count = htons((int)client_list.size());
    /* 发送在线用户数 */
    sendto(sock, (const char*)&count, sizeof (int), 0, (struct sockaddr*)&cliaddr, sizeof cliaddr);
    /* 发送在线用户列表 */
    for (USER_LIST::iterator it = client_list.begin(); it != client_list.end(); ++it)
    {
        sendto(sock, &*it, sizeof(USER_INFO), 0, (struct sockaddr*)&cliaddr, sizeof cliaddr);
    }
}

void chat_srv(int sock)
{
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    int n;
    MESSAGE msg;
    while (1)
    {
        memset(&msg, 0, sizeof(msg));
        clilen = sizeof cliaddr;
        n = recvfrom(sock, &msg, sizeof(msg), 0, (struct sockaddr *)&cliaddr, &clilen);
        if (n == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            ERR_EXIT("recvfrom")
        }

        int cmd = ntohs(msg.cmd);
        switch (cmd)
        {
            case C2S_LOGIN:
                do_login(msg, sock, cliaddr);
                break;
            case C2S_LOGOUT:
                do_logout(msg, sock, cliaddr);
                break;
            case C2S_ONLINE_USER:
                do_sendlist(sock, cliaddr);
                break;
            default:
                break;
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

    chat_srv(sock);

    return 0;
}
