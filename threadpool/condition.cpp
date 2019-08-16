//
// Created by jxq on 19-8-15.
//

#include "condition.h"
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int condition_init(condition_t *cond)
{
    int state;
    state = pthread_cond_init(&(cond->pcond), NULL);
    if (state == 0)
    {
        return state;
    }
    state = pthread_mutex_init(&(cond->pmutex), NULL);
    if (state == 0)
    {
        return state;
    }
    return 0;
}

int condition_lock(condition_t *cond)
{
    //cout << pthread_mutex_lock(&(cond->pmutex)) << endl;
    if (pthread_mutex_lock(&(cond->pmutex)) == 0)
    {
        printf("pthread_mutex_lock error\n");
        exit(EXIT_FAILURE);
    }
    return 1;
}

int condition_unlock(condition_t *cond)
{
    pthread_mutex_unlock(&(cond->pmutex));
}
int condition_wait(condition_t *cond)
{
    return pthread_cond_wait(&(cond->pcond), &(cond->pmutex));
}

int condition_timewait(condition_t *cond, const struct timespec *abstime)
{
    return pthread_cond_timedwait(&(cond->pcond), &(cond->pmutex), abstime);
}

int condition_signal(condition_t *cond)
{
    return pthread_cond_signal(&(cond->pcond));
}

int condition_broadcast(condition_t *cond)
{
    return pthread_cond_broadcast(&(cond->pcond));
}

int condition_destroy(condition_t *cond)
{
    int state;
    state = pthread_cond_destroy(&(cond->pcond));
    if (state == 0)
    {
        return state;
    }
    state = pthread_mutex_destroy(&(cond->pmutex));
    if (state == 0)
    {
        return state;
    }
    return 0;
}
