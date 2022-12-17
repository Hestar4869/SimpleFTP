//
// Created by Hestar on 2022/12/2.
//
#include <sys/socket.h>
#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include "server_util.h"
#include "../common/constant.h"

#define MAX_PID_NUM 10
// 声明一个简单的线程池结构,pid_list用于存储当前存活的线程id
// pid_num存储当前存活的线程数，alive[]用于标记可用的存储空间
pthread_t pid_list[MAX_PID_NUM];
int  pid_num,alive[MAX_PID_NUM];

//! 连接每个客户端的子线程主函数
void *pthread_main(int* socketfd);

int main(){
    int listen_socket = create_listen_server();
    if(listen_socket == -1){
        perror("listen_socket");
        return -1;
    }
    printf("当前服务器正在监听%d端口",PORT);

    while (1){
        // 等待一个ftp client接入
        int *socketfd=malloc(sizeof (int));
        *socketfd = wait_client(listen_socket);
        if(*socketfd == -1) return -1;

        // 寻找可用的pid存储空间
        pthread_t *pid;
        for (int i = 0; i < MAX_PID_NUM; ++i) {
            if(alive[i])    continue;
            alive[i]=1;
            pid = &(pid_list[i]);
            break;
        }

        pthread_create(pid,NULL,(void *)pthread_main,(void *)socketfd);

    }
    close(listen_socket);
}

void *pthread_main(int* socketfd){
    // 初始化该线程的工作路径
    char current_path[128];
    memset(current_path,0,sizeof (current_path));
    strcpy(current_path,"/home/hestar");

    while (1){
        char cmd_buffer[5];
        int num= recv(*socketfd,cmd_buffer,5,0);
        printf("收到%d个字节，命令%s\n",num,cmd_buffer);
        switch (str_to_cmd(cmd_buffer)) {
            case CMD_LS:
                execute_cmd_ls(*socketfd,current_path);
                break;
            case CMD_CD:
                execute_cmd_cd(*socketfd,current_path);
                break;
            case CMD_PUT:
                execute_cmd_put(*socketfd);
                break;
            case CMD_GET:
                execute_cmd_get(*socketfd);
                break;
            case CMD_QUIT:
                //表示可以退出该线程
                close(*socketfd);
                free(socketfd);
                pthread_exit(NULL);
        }
    }

}