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
#define PRODUCERS_COUNT 5
#define BUFFSIZE 10

int g_buffer[BUFFSIZE];

unsigned short in = 0;
unsigned short out = 0;
unsigned short produce_id = 0;
unsigned short consume_id = 0;

sem_t g_sem_full;
sem_t g_sem_empth;
pthread_mutex_t g_mutex;

pthread_t g_thread[CONSUMERS_COUNT + PRODUCERS_COUNT];

void *produce (void *arg)
{
    int num = *((int *)arg);
    int i;
    while (1)
    {
        printf("%d produce is waiting\n", num);
        sem_wait(&g_sem_full);
        pthread_mutex_lock(&g_mutex);

        for (i = 0; i < BUFFSIZE; ++i)
        {
            printf("%02d ", i);
            if (g_buffer[i] == -1)
            {
                printf("%s ", "null");
            }
            else
            {
                printf("%d ", g_buffer[i]);
            }
            if (i == in)
            {
                printf("\t<--produce");
            }
            printf("\n");
        }
        printf("%d produce begin produce product %d\n", num, produce_id);
        g_buffer[in] = produce_id;
        cout << g_buffer[in] << endl;
        in = (in + 1) % BUFFSIZE;
        printf("%d produce end produce product %d\n", num, produce_id++);
        pthread_mutex_unlock(&g_mutex);
        sem_post(&g_sem_empth);
        sleep(5);
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
        sem_wait(&g_sem_empth);
        pthread_mutex_lock(&g_mutex);

        for (i = 0; i < BUFFSIZE; ++i)
        {
            printf("%02d ", i);
            if (g_buffer[i] == -1)
            {
                printf("%s", "null");
            }
            else
            {
                printf("%d", g_buffer[i]);
            }
            if (i == out)
            {
                printf("\t<--consume");
            }
            printf("\n");
        }
        consume_id = g_buffer[out];
        printf("%d consume begin consume product %d\n", num, consume_id);
        g_buffer[out] = -1;
        out = (out + 1) % BUFFSIZE;
        printf("%d consume end consume product %d\n", num, consume_id);
        pthread_mutex_unlock(&g_mutex);
        sem_post(&g_sem_full);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char** argv)
{
    for (int i = 0; i < BUFFSIZE; ++i)
    {
        g_buffer[i] = -1;
    }

    sem_init(&g_sem_full, NULL, BUFFSIZE);
    sem_init(&g_sem_empth, NULL, 0);

    pthread_mutex_init(&g_mutex, NULL);

    int i;
    for (i = 0; i < CONSUMERS_COUNT; ++i)
    {
        pthread_create(&g_thread[i], NULL, consume, &i);
    }

    for (i = 0; i < PRODUCERS_COUNT; ++i)
    {
        pthread_create(&g_thread[CONSUMERS_COUNT+i], NULL, produce,  &i);
    }

    for (i = 0; i < CONSUMERS_COUNT + PRODUCERS_COUNT; ++i)
    {
        pthread_join(g_thread[i], NULL);
    }

    sem_destroy(&g_sem_full);
    sem_destroy(&g_sem_empth);
    pthread_mutex_destroy(&g_mutex);

    return 0;
}