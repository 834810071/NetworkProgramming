//
// Created by jxq on 19-8-14.
//

// p34 poxis消息队列

#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <cstring>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

using namespace std;

#define ERR_EXIT(m) \
        do  \
        {   \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while (0);

int main(int argc, char** argv)
{
    // mq_overview
    mqd_t mqid = mq_open("/abc",  O_RDONLY);
    if (mqid == ((mqd_t) -1))
    {
        ERR_EXIT("mq_open");
    }

    //printf("mqopen success\n");

    struct mq_attr attr;

    if (mq_getattr(mqid, &attr) == -1)
    {
        ERR_EXIT("mq_getattr");
    }
    printf("max #msg = %ld max #bytes/msg = %ld #currently on queue = %ld\n", attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

    mq_close(mqid);

    return 0;
}