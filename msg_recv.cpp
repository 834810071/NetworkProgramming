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
    int flag = 0;
    int type = 0;
    int opt;

    while (1)
    {
        opt = getopt(argc, argv, "nt:");
        if (opt == '?')
        {
            exit(EXIT_FAILURE);
        }
        if (opt == -1)
        {
            break;
        }

        switch (opt)
        {
            case 'n':
                flag |= IPC_NOWAIT;
                break;
            case 't':
                type = atoi(optarg);
                break;
        }
    }


    int msgid;
    msgid = msgget(1234, 0);
    if (msgid == -1)
    {
        ERR_EXIT("msgget");
    }

    struct msgbuf *ptr;
    ptr = (struct msgbuf*) malloc(sizeof(long) + 8192);
    ptr->mtype = type;
    //msgrcv( msqid, &buf1, recvlength ,3,0 ) ;
    int n;
    if ((n = msgrcv(msgid, ptr, 8192, type, flag)) < 0)
    {
        ERR_EXIT("msgsnd");
    }
    printf("type=%d, bytes=%d\n", ptr->mtype, (int)n) ;
    return 0;
}
