//
// Created by jxq on 19-8-12.
//

// p25 system v消息队列(二)

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define ERR_EXIT(m) \
        do \
        { \
             perror(m); \
             exit(EXIT_FAILURE);    \
        } while (0);


int main(int argc, char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <bytes> <type>\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    int len = atoi(argv[1]);
    int type = atoi(argv[2]);
    int msgid;
    msgid = msgget(1234, 0);
    if (msgid == -1)
    {
        ERR_EXIT("msgget");
    }

    struct msgbuf *ptr;
    ptr = (struct msgbuf*) malloc(sizeof(long) + len);
    ptr->mtype = type;
    if (msgsnd(msgid, ptr, len, 0) < 0)
    {
        ERR_EXIT("msgsnd");
    }

    return 0;
}
