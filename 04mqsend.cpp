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

struct student
{
    char name[32];
    int age;
};

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <msg_prio>\n", argv[1]);
    }

    // mq_overview
    mqd_t mqid = mq_open("/abc",  O_WRONLY);
    if (mqid == ((mqd_t) -1))
    {
        ERR_EXIT("mq_open");
    }

    //printf("mqopen success\n");
    student stu;
    strcpy(stu.name, "hello");
    stu.age = 20;

    if (mq_send(mqid, (const char*)&stu, sizeof(stu), atoi(argv[1])) == -1)
    {
        ERR_EXIT("mq_send");
    }

    printf("send success\n");
    mq_close(mqid);

    return 0;
}