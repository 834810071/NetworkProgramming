//
// Created by jxq on 19-8-12.
//

// p25 system v消息队列(三)

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <cstring>

#define ERR_EXIT(m) \
        do \
        { \
             perror(m); \
             exit(EXIT_FAILURE);    \
        } while (0);


#define MSGMAX 8192

void echo_srv(int msgid)
{
    struct msgbuf msg;
    int n;
    int type;
    memset(&msg, 0, sizeof(msgbuf));
    while (1)
    {
        if ((n = msgrcv(msgid, &msg, MSGMAX, 1, 0)) < 0)
        {
            ERR_EXIT("msgsnd");
        }

        type = *((int*)msg.mtext);
        msg.mtype = type;

        fputs(msg.mtext+4, stdout);

        if (msgsnd(msgid, &msg, n, 0) < 0)
        {
            ERR_EXIT("msgsnd");
        }
        //memset(&msg, 0, sizeof(msgbuf));
    }

}

int main(int argc, char** argv)
{
    int msgid;
    msgid = msgget(1234, 0);
    if (msgid == -1)
    {
        ERR_EXIT("msgget");
    }

    echo_srv(msgid);

    return 0;
}
