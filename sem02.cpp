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

// -c
int sem_creat(key_t key)
{
    int semid;
    semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid == -1)
    {
        ERR_EXIT("semget");
    }
    return semid;
}


int sem_open(key_t key)
{
    int semid;
    semid = semget(key, 0, 0);
    if (semid == -1)
    {
        ERR_EXIT("semget");
    }
    return semid;
}

// -s <val>
int sem_setval(int semid, int val)
{
    union semun su;
    su.val = val;
    int ret;
    ret = semctl(semid, 0, SETVAL, su);
    if (ret == -1)
    {
        ERR_EXIT("setval")
    }
    return 0;
}

// -g
int sem_getval(int semid)
{
    int ret;
    ret = semctl(semid, 0, GETVAL);
    if (ret == -1)
    {
        ERR_EXIT("getval")
    }
    printf("sem.val = %d\n", ret);
    return ret;
}

// -d
int sem_d(int semid)
{
    int ret;
    ret = semctl(semid, 0, IPC_RMID, NULL);
    if (ret == -1)
    {
        ERR_EXIT("rm_sem")
    }
    return 0;
}

// -p
int sem_p(int semid)
{
    struct sembuf sembuf;
    sembuf.sem_num = 0;
    sembuf.sem_op = -1;
    sembuf.sem_flg = 0;
    int ret;
    ret = semop(semid, &sembuf, 1);
    if (ret == -1)
    {
        ERR_EXIT("sem_p")
    }
    return ret;
}

// -v
int sem_v(int semid)
{
    struct sembuf sembuf;
    sembuf.sem_num = 0;
    sembuf.sem_op = 1;
    sembuf.sem_flg = 0;
    int ret;
    ret = semop(semid, &sembuf, 1);
    if (ret == -1)
    {
        ERR_EXIT("sem_v")
    }
    return ret;
}

// -f
int sem_getmode(int semid)
{
    union semun su;
    struct semid_ds sem;
    su.buf = &sem;
    int ret =semctl(semid, 0, IPC_STAT, su);
    if (ret == -1)
    {
        ERR_EXIT("semctl");
    }
    printf("current permissions is %o\n", su.buf->sem_perm.mode);

    return ret;
}


// -m <mode>
int sem_setmode(int semid, char* mode)
{
    union semun su;
    struct semid_ds sem;
    su.buf = &sem;

    int ret =semctl(semid, 0, IPC_STAT, su);
    if (ret == -1)
    {
        ERR_EXIT("semctl");
    }
    printf("current permissions is %o\n", su.buf->sem_perm.mode);

    sscanf(mode, "%o", (unsigned int*)&su.buf->sem_perm.mode);
    ret = semctl(semid, 0, IPC_SET, su);
    if (ret == -1)
    {
        ERR_EXIT("semctl");
    }

    printf("permission updated..\n");
    return ret;
}

void print(int sigmid, char* s)
{
    int i = 0;
    int pause_time;
    for (int i = 0; i < 10; ++i)
    {
        sem_p(sigmid);

        printf("%s", s);
        fflush(stdout);
        pause_time = rand() % 3;
        sleep(pause_time);
        printf("%s", s);
        fflush(stdout);

        sem_v(sigmid);

        pause_time = rand() % 2;
        sleep(pause_time);
    }
}

int main(int argc, char** argv)
{
    int sigmid = sem_creat(IPC_PRIVATE);
    sem_setval(sigmid, 0);
    pid_t pid = fork();

    if (pid == -1)
    {
        ERR_EXIT("fork");
    }

    if (pid > 0)
    {
        sem_setval(sigmid, 1);
        print(sigmid, "o");

    }
    else
    {
        print(sigmid, "x");
    }
    return 0;
}