//
// Created by jxq on 19-8-12.
//

// p28 共享内存介绍（一）

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>

#define ERR_EXIT(m) \
        do \
        { \
             perror(m); \
             exit(EXIT_FAILURE);    \
        } while (0);

typedef struct stu
{
    char name[4];
    int age;
} STU;

int main(int argc, char** argv)
{
    int fd;
    fd = open(argv[1], O_RDWR);
    if (fd == -1)
    {
        ERR_EXIT("open");
    }


    STU *p;
    p = (STU*)mmap(NULL, sizeof(STU) * 6, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);;
    if (p == NULL)
    {
        ERR_EXIT("mmap");
    }
    char ch = 'a';
    int i;
    for (int i = 0; i < 6; ++i)
    {
        printf("student's name = %s, age = %d\n", (p+i)->name, (p+i)->age);
//        memcpy((p+i)->name, &ch, 1);
//        (p+i)->age = 20 + i;
//        ++ch;
    }

    munmap(p, sizeof(STU)*6);
    printf("exit...\n");

    return 0;
}