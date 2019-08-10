//
// Created by jxq on 19-8-10.
//

// socket编程 17

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

#define ERR_EXIT(m) \
        do \
        { \
            perror(m); \
            exit(EXIT_FAILURE); \
        } while(0);


int main(int argc, char** argv)
{
    int fd[2];
    int r = socketpair(AF_UNIX, SOCK_STREAM, 0, fd);

    if (r < 0)
    {
        ERR_EXIT("socketpair");
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        int val = 0;
        close(fd[0]);
        while (1)
        {
            sleep(1);
            ++val;
            printf("sending data: %d\n", val);
            write(fd[1], &val, sizeof val);
            read(fd[1], &val, sizeof val);
            printf("recver data: %d\n", val);
        }
    }
    else
    {
        close(fd[1]);
        int val;
        while (1)
        {
            read(fd[0], &val, sizeof val);
            val++;
            write(fd[0], &val, sizeof val);
        }
    }
    return 0;
}