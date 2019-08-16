//
// Created by jxq on 19-8-15.
//

// p41 线程池

#include "condition.h"
#include "threadpool.h"
#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void *run (void *arg)
{
    printf("threadpool 0x%x working task %d\n", (int)pthread_self(), *(int*)(arg));
    sleep(1);
    free(arg);
    return NULL;
}

int main(void)
{
    // 结构体指针需要初始化 不晓得为啥报错
    threadpool_t pool;
    threadpool_init(&pool, 3);

    int i;
    for (i = 0; i < 10; ++i)
    {
        int *a = (int*)malloc(sizeof(int));
        *a = i;
        threadpool_add_task(&pool, run, a);
    }
   // sleep(15);
    threadpool_destroy(&pool);
    return 0;
}