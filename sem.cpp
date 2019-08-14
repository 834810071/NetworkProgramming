//
// Created by jxq on 19-8-13.
//

// p30 system v信号量（一）

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// https://blog.csdn.net/a1414345/article/details/64513946

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

void usage()
{
//    创建信号量 -c
    fprintf(stderr, "semtool -c\n");
//    删除	  -d
    fprintf(stderr, "semtool -d\n");
//    获得值    -g
    fprintf(stderr, "semtool -g\n");
//    设置值    -s <val>
    fprintf(stderr, "semtool -s <val>\n");
//    p操作	 -p
    fprintf(stderr, "semtool -p\n");
//    v操作     -v
    fprintf(stderr, "semtool -v\n");
//    查看权限   -f
    fprintf(stderr, "semtool -f\n");
//    更改权限   -m <mode>
    fprintf(stderr, "semtool -m <mode>\n");
}

int main(int argc, char** argv)
{
    int opt;

    opt = getopt(argc, argv, "cdgpvs:fm:");
    if (opt == '?')
    {
        exit(EXIT_FAILURE);
    }
    if (opt == -1)
    {
       usage();
       exit(EXIT_FAILURE);
    }

    // 为了获取一个独一无二的通信对象，必须使用键（可使用ftok()函数生成，返回值key）。
    // 这里的键是用来定位I P C 对象的标识符的
    key_t key = ftok(".", 's');
    int semid;
    switch (opt)
    {
        case 'c':
            sem_creat(key);
            break;
        case 'p':
            semid = sem_open(key);
            sem_p(semid);
            sem_getval(semid);
            break;
        case 'v':
            semid = sem_open(key);
            sem_v(semid);
            sem_getval(semid);
            break;
        case 'd':
            semid = sem_open(key);
            sem_d(semid);
            break;
        case 's':
            semid = sem_open(key);
            sem_setval(semid, atoi(optarg));
            break;
        case 'g':
            semid = sem_open(key);
            sem_getval(semid);
            break;
        case 'f':
            semid = sem_open(key);
            sem_getmode(semid);
            break;
        case 'm':
            semid = sem_open(key);
            sem_setmode(semid, argv[2]);
            break;
    }
    return 0;
}