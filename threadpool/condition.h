//
// Created by jxq on 19-8-15.
//

// p41 线程池

#ifndef NETWORKPROGRAMMING_CONDITION_H
#define NETWORKPROGRAMMING_CONDITION_H

#include <pthread.h>

typedef struct condition
{
    pthread_mutex_t pmutex;
    pthread_cond_t pcond;
} condition_t;

int condition_init(condition_t *cond);
int condition_lock(condition_t *cond);
int condition_unlock(condition_t *cond);
int condition_wait(condition_t *cond);
int condition_timewait(condition_t *cond, const struct timespec *abstime);
int condition_signal(condition_t *cond);
int condition_broadcast(condition_t *cond);
int condition_destroy(condition_t *cond);


#endif //NETWORKPROGRAMMING_CONDITION_H
