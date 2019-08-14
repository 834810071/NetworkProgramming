//
// Created by jxq on 19-8-14.
//

// p37 poxis 线程(一)

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

using namespace std;


#define ERR_EXIT(m) \
        do  \
        {   \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while(0);

void * start_routine (void *arg)
{
    pthread_detach(pthread_self());
    for (int i = 0; i < 20; ++i)
    {
        cout << "B";
    }
    return (char *)"hello";
}

int main(int argc, char** argv) {
    int ret;
    pthread_t thread;

    ret = pthread_create(&thread, NULL, start_routine, NULL);
    if (ret != 0)
    {
        ERR_EXIT("pthread_create");
    }

    for (int i = 0; i < 20; ++i)
    {
        cout << "A";
    }

    void *retval;
    if (pthread_join(thread, &retval) != 0)
    {
        ERR_EXIT("pthread_join");
    }
    printf("\n%s\n", (char *)retval);

    return 0;
}