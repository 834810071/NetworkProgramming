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

int echocli(int msgid)
{
    struct msgbuf msg;
    memset(&msg, 0, sizeof(msgbuf));
    int pid = getpid();
    msg.mtype = 1;
    *((int*)msg.mtext) = pid;
    int n;
    while (fgets(msg.mtext + 4, MSGMAX, stdin) != NULL)
    {
        if (msgsnd(msgid, &msg, sizeof(long) + strlen(msg.mtext), 0) < 0)
        {
            ERR_EXIT("msgsnd");
        }

        memset(msg.mtext+4, 0, sizeof(msg.mtext + 4));

        if ((n = msgrcv(msgid, &msg, MSGMAX, pid, 0)) < 0)
        {
            ERR_EXIT("msgsnd");
        }
        fputs(msg.mtext + 4, stdout);
        memset(msg.mtext + 4, 0, sizeof(msg.mtext + 4));
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

    echocli(msgid);



    return 0;
}
