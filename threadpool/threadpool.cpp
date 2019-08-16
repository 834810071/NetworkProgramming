//
// Created by jxq on 19-8-15.
//

// p41 线程池

#include "threadpool.h"
#include <pthread.h>
#include <memory>
#include <iostream>

using namespace std;

void *thread_routine (void *arg)
{
    struct timespec abstime;
    int timeout;
    printf("thread 0x%x is starting\n", (int)pthread_self());
    threadpool_t *pool = (threadpool_t*)arg;
    while (1)
    {
        timeout = 0;
        condition_lock(&pool->ready);
        ++pool->idle;
        // 等待队列有任务到来或者线程池销毁通知
        while (pool->first == NULL && !pool->quit)
        {
            printf("thread 0x%x is waiting\n", (int)pthread_self());
            //condition_wait(&pool->ready);
            clock_gettime(CLOCK_REALTIME, &abstime);
            abstime.tv_sec += 2;
            // 超时设置2秒
            int state = condition_timewait(&pool->ready, &abstime);
            if (state == ETIMEDOUT)
            {
                printf("thread 0x%x is wait time out\n", (int)pthread_self());
                timeout = 1;
                break;
            }
        }
        // 等待到条件，处于工作状态
        --pool->idle;
        if (pool->first != NULL)
        {
            // 从队头取出任务
            task_t *t = pool->first;
            pool->first = t->next;
            // 执行任务需要一定的时间，所以需要先解锁，以便生产者进程
            // 能够往队列中天剑任务，其他消费者线程能够进入等待任务
            condition_unlock(&pool->ready);
            t->run(t->arg);
            free(t);
            condition_lock(&pool->ready);
        }
        // 如果等待到销毁线程池通知，且任务都执行完毕
        if (pool->quit && pool->first == NULL)
        {
            pool->counter--;
            if (pool->counter == 0)
            {
                condition_signal(&pool->ready);
            }
            condition_unlock(&pool->ready);
            // 跳出循环之前记得解锁
            break;
        }

        if (timeout && pool->first == NULL)
        {
            pool->counter--;
            condition_unlock(&pool->ready);
            // 跳出循环之前记得解锁
            break;
        }

        condition_unlock(&pool->ready);
    }

    printf("thread 0x%x is exiting \n", (int)pthread_self());

    return NULL;
}

// 初始化线程池
void threadpool_init(threadpool_t *pool, int threads)
{
   // pool = (threadpool_t*)malloc(sizeof(pool));
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
    // 生成新任务
    task_t* newtask = (task_t *)malloc(sizeof(task_t));
    //cout << (char *)pool->first->arg << endl;
    newtask->run = run;
    newtask->arg = arg;
    newtask->next = NULL;


    condition_lock(&pool->ready);
    // 将任务添加到队列中
    if (pool->first == NULL)
    {
        pool->first = newtask;
    }
    else
    {
        pool->last->next = newtask;
    }
    pool->last = newtask;

    // 如果有等待任务的线程，则唤醒其中一个
    if (pool->idle > 0)
    {
        condition_signal(&pool->ready);
    }
    else if (pool->counter < pool->max_threads)
    {
        // 没有等待线程，并且当前线程数不超过最大线程数,则创建一个新线程
        pthread_t tid;
        pthread_create(&tid, NULL, thread_routine, pool);
        ++pool->counter;
    }
    condition_unlock(&pool->ready);
}

// 销毁线程池
void threadpool_destroy(threadpool_t *pool)
{
    if (pool->quit)
    {
        return;
    }
    condition_lock(&pool->ready);
    pool->quit = 1;
    if (pool->counter > 0)
    {
        if (pool->idle > 0)
        {
            condition_broadcast(&pool->ready);
        }
        // 处于执行任务状态中的线程，不会收到广播
        // 线程池需要等执行任务状态中的线程全部退出
        while (pool->counter > 0)
        {
            condition_wait(&pool->ready);
        }
    }
    condition_unlock(&pool->ready);
    condition_destroy(&pool->ready);
    //free(pool);
}