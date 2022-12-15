//
// Created by Hestar on 2022/12/2.
//

#include "client_util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int connect_server(int socketfd, char *host, int port) {
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = PF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(port);

    int chk;
    chk = connect(socketfd, (const struct sockaddr *) (struct sockaddr_in *) &server_addr, sizeof (struct  sockaddr_in));

    // 0 正常，-1 错误
    return chk;
}

int send_cmd_ls(int socketfd) {
    //接受传送过来的socket参数,发送CMD_LS宏信息
    int num;
    num = send(socketfd,"ls", 3,0);

    //TODO 等待响应ACK码

    //收到ls命令信息

    //打印控制台
    return num;
}

int send_cmd_cd(int socketfd) {
    //利用传送过来的socket参数,发送CMD_CD宏信息
    int num;
    num = send(socketfd,"cd", 3,0);

    //等待响应ACK码
    // code here

    //收到cd命令信息

    //打印控制台
    return  num;
}

int send_cmd_put(int socketfd) {
    //利用传递的socket参数 ,local_file ,remote_file，发送CMD_PUT宏信息
    int num;
    num = send(socketfd,"put", 4,0);
    //等待响应ACK码

    //开启新线程并创建新socket连接，传输端口

    //利用common/transfer.h中方法，发送文件

    //等待结束，并打印信息
    return  num;
}

int send_cmd_get(int socketfd) {
    int num;
    num = send(socketfd,"get", 4,0);
    return  num;
}

int send_cmd_quit(int socketfd) {
    // 发送CMD_QUIT
    int num;
    num = send(socketfd,"quit", 5,0);
    //等待响应

    //原地释放资源 ， 或函数返回后释放
    return  num;
}




