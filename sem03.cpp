//
// Created by jxq on 19-8-13.
//

// p30 system v信号量（二）

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <wait.h>

#define ERR_EXIT(m) \
        do \
        { \
             perror(m); \
             exit(EXIT_FAILURE);    \
        } while (0);

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};

int sigmid;

void get2fork(int pid)
{
    unsigned short int left = pid;
    unsigned short int right = (pid + 1) % 5;
    struct sembuf sembuf[2] = {{left, -1, 0}, {right, -1, 0}};
    int ret = semop(sigmid, sembuf, 2);
    if (ret == -1)
    {
        ERR_EXIT("get2fork")
    }
};

void put2fork(int pid)
{
    unsigned short int left = pid;
    unsigned short int right = (pid + 1) % 5;
    struct sembuf sembuf[2] = {{left, 1, 0}, {right, 1, 0}};
    int ret = semop(sigmid, sembuf, 2);
    if (ret == -1)
    {
        ERR_EXIT("get2fork")
    }
};


void philosopher(int no)
{
    while (1)
    {
        printf("%d is thinking\n", no);
        sleep(rand() % 3);
        printf("%d is hungry\n", no);
        get2fork(no);
        printf("%d is eating\n", no);
        sleep(rand() % 3);
        put2fork(no);
    }
}


int main(int argc, char** argv)
{
    sigmid = semget(IPC_PRIVATE, 5, IPC_CREAT | IPC_EXCL | 0666);
    if (sigmid == -1)
    {
        ERR_EXIT("sigmid");
    }

    union semun su;
    su.val = 1;
    for (int i = 0; i < 5; ++i)
    {
        semctl(sigmid, i, SETVAL, su);
    }

    int no = 0;
    pid_t pid;
    for (int i = 1; i < 5; ++i)
    {
        pid = fork();
        if (pid > 0)
        {
            no = i;
            break;
        }
    }

    philosopher(no);
    return 0;
}