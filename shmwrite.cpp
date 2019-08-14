//
// Created by jxq on 19-8-13.
//

// p29 system v共享内存

#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <string>
#include <errno.h>
#include <cstring>

using namespace std;

#define ERR_EXIT(m) \
        do \
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
    int shmid;  // 共享内存标识符

    // 创建共享内存
    shmid = shmget((key_t)1234, sizeof(student), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        ERR_EXIT("shmget");
    }

    // 第一次创建完共享内存时，它还不能被任何进程访问，shmat()函数的作用就是用来启动对该共享内存的访问，并把共享内存连接到当前进程的地址空间
    // 将共享内存链接到当前进程的地址空间
    void *shm = shmat(shmid, NULL, 0);
    if (shm == (void *)-1)
    {
        ERR_EXIT("shmat");
    }

    // 设置共享内存
    student *shared = (struct student*) shm;
    strcpy(shared->name, "hello");
    shared->age = 20;

    while (true)
    {
        if (memcmp(shared->name, "quit", 4) == 0)
        {
            break;
        }
    }
    // 把共享内存从当前进程中分离
    if (shmdt(shm) == -1)
    {
        ERR_EXIT("shmdt");
    }

    shmctl(shmid, IPC_RMID, 0);
    return 0;
}
