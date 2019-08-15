//
// Created by jxq on 19-8-15.
//

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
}

int main(void)
{
    threadpool_t *pool;
    threadpool_init(pool, 3);

    int i;
    for (i = 0; i < 10; ++i)
    {
        int *a = &i;
        threadpool_add_task(pool, run, a);
    }

    threadpool_destroy(pool);
    return 0;
}