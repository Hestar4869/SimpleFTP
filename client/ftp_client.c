//
// Created by Hestar on 2022/12/2.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "client_util.h"

int main(int argc,char *argv[]){
    char* host ;
    int port;
    // 客户端连接时，用ftp [host] [port]
    if(argc == 3){
        host = argv[1];
        port = atoi(argv[2]);
        printf("正在连接主机%s:%d......\n",host,port);
    }
    else{
        sprintf(stderr,"输入参数数目错误");
        exit(-1);
    }

    // 创建socket并连接
    int socketfd;
    socketfd = socket(AF_INET , SOCK_STREAM , 0);
    if(socketfd == -1){
        perror("client socket error");
        return -1;
    }
    int chk;

    // 连接服务器
    chk = connect_server(socketfd,host,port);
    if(chk == -1){
        perror("client connecting error");
        return -1;
    }
    printf("成功连接主机%s\n",host);

    //循环读取命令
    while (1){
        printf("ftp> ");
        char str_cmd[5];
        scanf("%s",str_cmd);
        switch (str_to_cmd(str_cmd)) {
            case CMD_LS:
                send_cmd_ls(socketfd);
                break;
            case CMD_CD:
                send_cmd_cd(socketfd);
                break;
            case CMD_GET:
                send_cmd_get(socketfd);
                break;
            case CMD_PUT:
                send_cmd_put(socketfd);
                break;
            case CMD_QUIT:
                send_cmd_quit(socketfd);
                close(socketfd);
                return 0;
        }
    }


    return 0;
}