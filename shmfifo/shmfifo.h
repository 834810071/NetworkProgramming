//
// Created by jxq on 19-8-13.
//

#ifndef NETWORKPROGRAMMING_SHMFIFO_H
#define NETWORKPROGRAMMING_SHMFIFO_H

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

typedef struct shmfifo shmfifo_t;
typedef struct shmread shmhead_t;

struct shmread
{
    unsigned int blocksize; // 块大小
    unsigned int blocks;    // 总块数
    unsigned int rd_index;  // 读索引
    unsigned int wr_index;  // 写索引
};

struct shmfifo
{
    shmhead_t *p_shm;   // 共享内存头部指针
    char* p_payloadd;   // 有效负载的起始地址

    int shmid;          // 共享内存ID
    int sem_mutex;      // 用来互斥用的信号量
    int sem_full;       // 用来控制共享内参是否满的信号量
    int sem_empty;       // 用来控制共享内参是否空的信号量
};

shmfifo* shmfifo_init(int key, int blocksize, int blocks);
void shmfifo_put(shmfifo* fifo, const void* buf);
void shmfifo_get(shmfifo* fifo, char* buf);
void shmfifo_destroy(shmfifo* fifo);

#endif //NETWORKPROGRAMMING_SHMFIFO_H
