//
// Created by jxq on 19-8-8.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 名字长度包含'\0'
#define _INT_NAME (64)
// 报文最大长度,包含'\0'
#define _INT_TEXT (512)

//4.0 控制台打印错误信息, fmt必须是双引号括起来的宏
#define CERR(fmt, ...) \
    fprintf(stderr,"[%s:%s:%d][error %d:%s]" fmt "\r\n",\
         __FILE__, __func__, __LINE__, errno, strerror(errno),##__VA_ARGS__)

//4.1 控制台打印错误信息并退出, t同样fmt必须是 ""括起来的字符串常量
#define CERR_EXIT(fmt,...) \
    CERR(fmt,##__VA_ARGS__),exit(EXIT_FAILURE)
/*
 * 简单的Linux上API错误判断检测宏, 好用值得使用
 */
#define IF_CHECK(code) \
    if((code) < 0) \
        CERR_EXIT(#code)

// 发送和接收的信息体
struct umsg{
    char type;                //协议 '1' => 向服务器发送名字, '2' => 向服务器发送信息, '3' => 向服务器发送退出信息
    char name[_INT_NAME];    //保存用户名字
    char text[_INT_TEXT];    //得到文本信息,空间换时间
};

/*
 * udp聊天室的客户端, 子进程发送信息,父进程接受信息
 */
int main(int argc, char* argv[]) {
    int sd, rt;
    struct sockaddr_in addr = { AF_INET };
    socklen_t alen = sizeof addr;
    pid_t pid;
    struct umsg msg = { '1' };

    // 这里简单检测
    if(argc != 4) {
        fprintf(stderr, "uage : %s [ip] [port] [name]\n", argv[0]);
        exit(-1);
    }
    // 下面对接数据
    if((rt = atoi(argv[2]))<1024 || rt > 65535)
        CERR("atoi port = %s is error!", argv[2]);
    // 接着判断ip数据
    IF_CHECK(inet_aton(argv[1], &addr.sin_addr));
    addr.sin_port = htons(rt);
    // 这里拼接用户名字
    strncpy(msg.name, argv[3], _INT_NAME - 1);

    //创建socket 连接
    IF_CHECK(sd = socket(PF_INET, SOCK_DGRAM, 0));
    // 这里就是发送登录信息给udp聊天服务器了
    IF_CHECK(sendto(sd, &msg, sizeof msg, 0, (struct sockaddr*)&addr, alen));

    //开启一个进程, 子进程处理发送信息, 父进程接收信息
    IF_CHECK(pid = fork());
    if(pid == 0) { //子进程,先忽略退出处理防止成为僵尸进程
        signal(SIGCHLD, SIG_IGN);
        while(fgets(msg.text, _INT_TEXT, stdin)){
            if(strcasecmp(msg.text, "quit\n") == 0){ //表示退出
                msg.type = '3';
                // 发送数据并检测
                IF_CHECK(sendto(sd, &msg, sizeof msg, 0, (struct sockaddr*)&addr, alen));
                break;
            }
            // 洗唛按发送普通信息
            msg.type = '2';
            IF_CHECK(sendto(sd, &msg, sizeof msg, 0, (struct sockaddr*)&addr, alen));
        }
        // 处理结算操作,并杀死父进程
        close(sd);
        kill(getppid(), SIGKILL);
        exit(0);
    }
    // 这里是父进程处理数据的读取
    for(;;){
        bzero(&msg, sizeof msg);
        IF_CHECK(recvfrom(sd, &msg, sizeof msg, 0, (struct sockaddr*)&addr, &alen));
        msg.name[_INT_NAME-1] = msg.text[_INT_TEXT-1] = '\0';
        switch(msg.type){
            case '1':printf("%s 登录了聊天室!\n", msg.name);break;
            case '2':printf("%s 说了: %s\n", msg.name, msg.text);break;
            case '3':printf("%s 退出了聊天室!\n", msg.name);break;
            default://未识别的异常报文,程序直接退出
                fprintf(stderr, "msg is error! [%s:%d] => [%c:%s:%s]\n", inet_ntoa(addr.sin_addr),
                        ntohs(addr.sin_port), msg.type, msg.name, msg.text);
                goto __exit;
        }
    }

    __exit:
    // 杀死并等待子进程退出
    close(sd);
    kill(pid, SIGKILL);
    waitpid(pid, NULL, -1);

    return 0;
}