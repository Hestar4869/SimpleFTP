//
// Created by Hestar on 2022/12/2.
//

#include "client_util.h"
#include "../common/transfer_file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <libgen.h>

int connect_server(int socketfd, char *host, int port) {
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = PF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(port);

    int chk;
    chk = connect(socketfd, (const struct sockaddr *) (struct sockaddr_in *) &server_addr, sizeof(struct sockaddr_in));

    // 0 正常，-1 错误
    return chk;
}

int send_cmd_ls(int socketfd) {
    //接受传送过来的socket参数,发送CMD_LS宏信息
    int num, buffer_len;
    char cmd[5], buffer[1024];
    memset(&cmd, 0, sizeof(cmd));
    strncpy(cmd, "ls", 2);
    num = send(socketfd, cmd, 5, 0);

    //收到ls命令信息
    buffer_len = recv(socketfd, buffer,1024,0);

    //打印控制台
    int file_num=0;
    char *line = strtok(buffer," ");
    while(line){
        file_num++;
        printf("%s ",line);
        if(file_num%5==0) printf("\n");
        line= strtok(NULL," ");
    }
    printf("\n该目录下总共有%d个文件项\n",file_num);

    return num;
}

int send_cmd_cd(int socketfd) {
    //利用传送过来的socket参数,发送CMD_CD宏信息
    int num;
    char cmd[5];
    memset(&cmd, 0, sizeof(cmd));
    strncpy(cmd, "cd", 2);
    num = send(socketfd, cmd, 5, 0);

    // 发送新的路径参数
    char newpath[128];
    memset(newpath,0,sizeof (newpath));
    scanf("%s",newpath);
    num += send(socketfd,newpath,128,0);

    //收到cd命令信息
    char buffer[128];
    int buffer_len;
    buffer_len = recv(socketfd,buffer,128,0);
    printf("now the path is %s\n",buffer);
    //打印控制台
    return num;
}

int send_cmd_put(int socketfd) {
    //利用传递的socket参数 ,local_file ,remote_file，发送CMD_PUT宏信息
    int num;
    char cmd[5];
    memset(&cmd, 0, sizeof(cmd));
    strncpy(cmd, "put", 3);
    num = send(socketfd, cmd, 5, 0);

    //初始化put命令读入的参数
    char local_file[128] , remote_file[128];
    memset(local_file,0,sizeof (local_file));
    memset(remote_file,0,sizeof (remote_file));
    scanf("%s %s",local_file,remote_file);
    // 检测本地文件是否存在
    if(access(local_file,0)) {
        printf("本地文件%s不存在", local_file);
        return -1;
    }

    // 传递远程文件名并判断是否存在
    send(socketfd,remote_file,128,0);
    char msg_flag[2];
    recv(socketfd,msg_flag,3,0);
    if(!strcmp(msg_flag,NET_MSG_ERROR) ){
        dirname(remote_file);
        printf("远程路径%s不存在\n",remote_file);
        return -1;
    }

    // 读取本地文件并上传
    upload(socketfd,local_file);
    //开启新线程并创建新socket连接，传输端口

    //利用common/transfer.h中方法，发送文件

    //等待结束，并打印信息
    return num;
}

int send_cmd_get(int socketfd) {

    //初始化get命令读入的参数
    char local_file[128] , remote_file[128];
    memset(local_file,0,sizeof (local_file));
    memset(remote_file,0,sizeof (remote_file));
    scanf("%s %s",remote_file,local_file);

    // 检测本地文件所在目录是否存在
    char* dir;
    dir = strdup(local_file);
    dirname(dir);
    if(access(dir,0)){
        printf("文件%s所在目录并不存在\n",dir);
        return -1;
    }

    // 发送请求文件的命令
    int num;
    char cmd[5];
    memset(&cmd, 0, sizeof(cmd));
    strncpy(cmd, "get", 3);
    num = send(socketfd, cmd, 5, 0);

    // 传递远程文件名并判断是否存在
    send(socketfd,remote_file,128,0);
    char msg_flag[2];
    recv(socketfd,msg_flag,3,0);
    if(!strcmp(msg_flag,NET_MSG_ERROR) ){
        dirname(remote_file);
        printf("所请求的远程文件%s不存在\n",remote_file);
        return -1;
    }

    download(socketfd,local_file);
    return num;
}

int send_cmd_quit(int socketfd) {
    // 发送CMD_QUIT
    int num;
    char cmd[5];
    memset(&cmd, 0, sizeof(cmd));
    strncpy(cmd, "quit", 4);
    num = send(socketfd, cmd, 5, 0);
    //等待响应

    //原地释放资源 ， 或函数返回后释放
    return num;
}




