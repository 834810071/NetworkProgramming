//
// Created by jxq on 19-8-8.
//

#ifndef NETWORKPROGRAMMING_PUB_H
#define NETWORKPROGRAMMING_PUB_H

#include <list>
#include <algorithm>

using namespace std;

// C2S
#define C2S_LOGIN               0x01
#define C2S_LOGOUT              0x02
#define C2S_ONLINE_USER         0x03

#define MSG_LEN                 512

// S2C
#define S2C_LOGIN_OK            0x01
#define S2C_ALREADY_LOGINED     0x02
#define S2C_SOMEONE_LOGIN       0x03
#define S2C_SOMEONE_LOGOUT      0x04
#define S2C_ONLINE_USER         0x05

// C2C
#define C2C_CHAT                0x06

typedef struct message
{
    int cmd;
    char body[MSG_LEN];
} MESSAGE;

typedef struct user_info
{
    char username[16];
    unsigned int ip;
    unsigned short port;
} USER_INFO;

typedef struct char_msg
{
    char username[16];
    char msg[100];
} CHAT_MSG;

typedef list<USER_INFO> USER_LIST;

#endif //NETWORKPROGRAMMING_PUB_H
