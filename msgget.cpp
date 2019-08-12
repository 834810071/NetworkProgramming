//
// Created by jxq on 19-8-12.
//

// p25 system v消息队列(一)

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

int main(void)
{
    int msgid;
    // 当只有IPC_EXCL选项打开时，不管有没有该快共享内存，shmget()都返回-1

    // 当只有IPC_CREAT选项打开时,不管是否已存在该块共享内存，则都返回该共享内存的ID，若不存在则创建共享内存
    msgid = msgget(1234, 0666 | IPC_CREAT);
    // 当IPC_CREAT | IPC_EXCL时, 如果没有该块共享内存，则创建，并返回共享内存ID。若已有该块共享内存，则返回-1；

    //msgid = msgget(1234, 0666 | IPC_CREAT | IPC_EXCL);
     //msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT | IPC_EXCL);
    /*msgid = msgget(IPC_PRIVATE, 0666);
    msgid = msgget(1234, 0666 | IPC_CREAT);*/
    //msgid = msgget(1234, 0);    // flags = 0 表示按原来权限打开

    if (msgid == -1)
    {
        ERR_EXIT("msgget");
    }

    printf("msgget successful\n");
    printf("msgid = %d\n", msgid);

    return 0;
}