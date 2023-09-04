//
// Created by Hestar on 2022/12/2.
//
#include <sys/socket.h>
#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include<sys/epoll.h>
#include <errno.h>
#include <bits/sigaction.h>
#include <signal.h>
#include <map>
#include "server_util.h"
#include "../common/constant.h"
#include"task.h"
#include "threadpool.h"

#define MAX_EVENT_NUMBER 1024
#define FD_LIMIT 65535
#define TIMESHOT 5

#define MAX_PID_NUM 10
// 声明一个简单的线程池结构,pid_list用于存储当前存活的线程id
// pid_num存储当前存活的线程数，alive[]用于标记可用的存储空间
pthread_t pid_list[MAX_PID_NUM];
int  pid_num,alive[MAX_PID_NUM];

// 传递信号的管道
static int pipefd[2];

//! 连接每个客户端的子线程主函数
void *pthread_main(int* socketfd);

int setnonblocking(int fd);
void addfd(int epollfd,int fd);
//! 信号处理函数
void sig_handler(int sig);
//! 设置信号的处理函数
void addsig(int sig);

int main(){
    //建立线程池
    threadpool<Task>* pool=NULL;
    try {
        pool = new threadpool<Task>;
    }
    catch (...){
        return 1;
    }

    // 创建监听socket
    int listenfd = create_listen_server();
    if(listenfd == -1){
        perror("listenfd");
        return -1;
    }
    printf("当前服务器正在监听%d端口",PORT);

    struct epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    addfd(epollfd, listenfd);

    int ret = socketpair(PF_UNIX,SOCK_STREAM,0,pipefd);
    setnonblocking(pipefd[1]);
    addfd(epollfd,pipefd[0]);

    addsig(SIGHUP);
    addsig(SIGCHLD);
    addsig(SIGTERM);
    addsig(SIGINT);

    // 为每个套接字创建一个对应的task
    std::map<int,Task*> tasks;

    bool stop_server = false;

    while (!stop_server){
        int number = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        if(number<0 && errno!=EINTR){
            printf("epoll failure\n");
            break;
        }
        for(int i=0;i<number;i++){
            int sockfd = events[i].data.fd;
            //! 有新连接
            if(sockfd == listenfd){
                int connfd = wait_client(listenfd);
                // 将新建立的连接，加入到监听表中
                addfd(epollfd,connfd);

                Task* task = new Task(connfd);
                tasks[connfd] = task;
            }
            //! 有信号，统一事件源
            else if((sockfd == pipefd[0])&&(events[i].events&EPOLLIN)){
                int sig;
                char signals[1024];
                ret = recv(pipefd[0],signals,sizeof(signals),0);
                if(ret == -1){
                    continue;
                }
                else if(ret == 0){
                    // 管道关闭
                    continue;
                }
                else{
                    for(int i=0;i<ret;++i){
                        switch (signals[i]) {
                            case SIGCHLD:
                            case SIGHUP:
                                continue;
                            case SIGTERM:
                            case SIGINT:
                                stop_server = true;
                        }
                    }
                }
            }
            //! 连接的socket中，有内容传输
            else{
                // 将sockfd对应的任务传入线程池的请求队列
                pool->append(tasks[sockfd]);
            }
        }

        // 等待一个ftp client接入
        int *socketfd=malloc(sizeof (int));
        *socketfd = wait_client(listenfd);
        if(*socketfd == -1) return -1;

        // 寻找可用的pid存储空间
        pthread_t *pid;
        for (int i = 0; i < MAX_PID_NUM; ++i) {
            if(alive[i])    continue;
            alive[i]=1;
            pid = &(pid_list[i]);
            break;
        }

        // 创建一个新线程处理新连接的客户端
        pthread_create(pid,NULL,(void *)pthread_main,(void *)socketfd);
    }
    close(listenfd);
}
int setnonblocking(int fd){
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}
void addfd(int epollfd,int fd){
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN|EPOLLET;
    setnonblocking(fd);
}
void sig_handler(int sig){
    int save_errno = errno;
    int msg = sig;
    // 将信号量写入管道，以通知主循环
    send(pipefd[1],(char*)&msg,1,0);
    errno = save_errno;
}
void addsig(int sig){
    struct sigaction sa;
    memset(&sa,0,sizeof(sa));
    sa.sa_flags |= SA_RESTART;
    sa.sa_handler = sig_handler;
    sigfillset(&sa.sa_mask);
    sigaction(sig,&sa,NULL);
}

void *pthread_main(int sockfd){
    // 初始化该线程的工作路径
    char current_path[128];
    memset(current_path,0,sizeof (current_path));
    strcpy(current_path,"/home/hestar");

    while (1){
        char cmd_buffer[5];
        int num= recv(sockfd, cmd_buffer, 5, 0);
        printf("收到%d个字节，命令%s\n",num,cmd_buffer);
        switch (str_to_cmd(cmd_buffer)) {
            case CMD_LS:
                execute_cmd_ls(sockfd, current_path);
                break;
            case CMD_CD:
                execute_cmd_cd(sockfd, current_path);
                break;
            case CMD_PUT:
                execute_cmd_put(sockfd);
                break;
            case CMD_GET:
                execute_cmd_get(sockfd);
                break;
            case CMD_QUIT:
                //表示可以退出该线程
                close(sockfd);

                pthread_exit(NULL);
        }
    }

}