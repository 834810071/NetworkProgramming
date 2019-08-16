//
// Created by jxq on 19-8-15.
//

// p41 线程池

#ifndef NETWORKPROGRAMMING_THREADPOOL_H
#define NETWORKPROGRAMMING_THREADPOOL_H

#include "condition.h"

// 任务结构体，将任务放入队列由线程池中的线程来执行
typedef struct task
{
    void *(*run)(void *arg);    // 任务回调函数
    void *arg;                  // 回调函数参数
    struct task* next;
} task_t;

// 线程池结构体
typedef struct threadpool
{
    condition_t ready;  // 任务准备就绪或者线程池销毁通知
    task_t *first;      // 任务队列头指针
    task_t *last;       // 任务队列尾指针
    int counter;        // 线程池当前线程数
    int idle;           // 线程池中当前正在等待任务的线程数
    int max_threads;    // 线程池中最大允许的线程数
    int quit;           // 销毁线程池的时候置1
} threadpool_t;

// 初始化线程池
void threadpool_init(threadpool_t *pool, int threads);

// 在线程池中添加任务
void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg);

// 销毁线程池
void threadpool_destroy(threadpool_t *pool);


#endif //NETWORKPROGRAMMING_THREADPOOL_H
