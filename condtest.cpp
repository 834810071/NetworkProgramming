//
// Created by jxq on 19-8-15.
//

// p39 posix信号量与互斥锁

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <pthread.h>

using namespace std;

#define ERR_EXIT(m) \
        do  \
        {   \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while(0);

#define CONSUMERS_COUNT 1
#define PRODUCERS_COUNT 1

pthread_mutex_t g_mutex;
pthread_cond_t g_cond;

pthread_t g_thread[CONSUMERS_COUNT + PRODUCERS_COUNT];

int ready = 0;

void *produce (void *arg)
{
    int num = *((int *)arg);
    int i;
    while (1)
    {
        printf("%d produce is waiting\n", num);
        pthread_mutex_lock(&g_mutex);
        printf("%d produce begin produce product %d\n", num, ready);

        while (ready == 0)
        {
            pthread_cond_wait(&g_cond, &g_mutex);
        }

        printf("%d produce end produce product %d\n", num, ready++);
        pthread_mutex_unlock(&g_mutex);
        sleep(1);
    }
    return NULL;
}

void *consume (void *arg)
{
    int num = *((int *)arg);
    int i;
    while (1)
    {
        printf("%d consume is waiting\n", num);
        pthread_mutex_lock(&g_mutex);
        printf("%d consume begin consume product %d\n", num, ready);

        ++ready;
        pthread_cond_signal(&g_cond);

        printf("%d consume end consume product %d\n", num, ready);
        pthread_mutex_unlock(&g_mutex);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char** argv)
{

    pthread_mutex_init(&g_mutex, NULL);
    pthread_cond_init(&g_cond, NULL);

    int i;
    for (i = 0; i < CONSUMERS_COUNT; ++i)
    {
        pthread_create(&g_thread[i], NULL, consume, &i);
    }
    sleep(1);
    for (i = 0; i < PRODUCERS_COUNT; ++i)
    {
        pthread_create(&g_thread[CONSUMERS_COUNT+i], NULL, produce,  &i);
    }

    for (i = 0; i < CONSUMERS_COUNT + PRODUCERS_COUNT; ++i)
    {
        pthread_join(g_thread[i], NULL);
    }

    pthread_mutex_destroy(&g_mutex);
    pthread_cond_destroy(&g_cond);

    return 0;
}