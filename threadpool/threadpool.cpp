//
// Created by jxq on 19-8-15.
//

#include "threadpool.h"
#include <pthread.h>

// 初始化线程池
void threadpool_init(threadpool_t *pool, int threads)
{
    condition_init(&(pool->ready));
    pool->first = NULL;
    pool->last = NULL;
    pool->counter = 0;
    pool->idle = 0;
    pool->max_threads = threads;
    pool->quit = 0;

}

// 在线程池中添加任务
void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg)
{
    task_t newtask;
    newtask.run = run;
    newtask.arg = arg;
    newtask.next = NULL;
}

// 销毁线程池
void threadpool_destroy(threadpool_t *pool)
{

}