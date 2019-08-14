//
// Created by jxq on 19-8-13.
//

// p33 system v共享内存与信号量综合

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "shmfifo.h"

#define ERR_EXIT(m) \
        do \
        { \
             perror(m); \
             exit(EXIT_FAILURE);    \
        } while (0);

struct student
{
    char name[32];
    int age;
};

int main(int argc, char** argv)
{
    shmfifo_t* shmfifo1 = shmfifo_init(1234, sizeof(student), 3);

    student* s = (struct student*)malloc(sizeof(student));
    memset(s, 0, sizeof(student));
    s->age = 20;
    s->name[0] = 'A';
    for(int i = 0; i < 5; ++i)
    {

        shmfifo_put(shmfifo1, s);
        s->name[0] = s->name[0] + i + 1;
        s->age = s->age + i + 1;
        printf("send ok\n");
    }

    //shmfifo_destroy(shmfifo1);
    return 0;
}