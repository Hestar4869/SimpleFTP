//
// Created by Hestar on 2022/12/16.
//

#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <libgen.h>
#include <unistd.h>
#include "server_util.h"
#include "../common/constant.h"
#include "../common/transfer_file.h"


int create_listen_server() {
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket == -1) {
        perror("create listen_socket");
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;  /* Internet地址族 */
    addr.sin_port = htons(PORT);  /* 端口号 */
    addr.sin_addr.s_addr = htonl(INADDR_ANY);   /* IP地址 */

    // close socket（一般不会立即关闭而经历TIME_WAIT的过程）后想继续重用该socket
    int bReuseaddr=1;
    if(setsockopt(listen_socket,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(int))<0)
    {
        perror("setsockopt failed");
        return -1;
    }

    // 绑定端口
    int ret = bind(listen_socket, (struct sockaddr *) &addr, sizeof(addr));
    if (ret == -1) {
        perror("listen_socket bind");
        return -1;
    }

    // 监听
    ret = listen(listen_socket, 5);
    if (ret == -1) {
        perror("listen");
        return -1;
    }
    return listen_socket;
}

int wait_client(int listen_socket) {
    struct sockaddr_in cliaddr;
    int addrlen = sizeof(cliaddr);
    printf("等待客户端连接。。。。\n");
    int client_socket = accept(listen_socket, (struct sockaddr *)&cliaddr, &addrlen);   //创建一个和客户端交流的套接字
    if(client_socket == -1)
    {
        perror("accept");
        return -1;
    }

    printf("成功接收到一个客户端：%s\n", inet_ntoa(cliaddr.sin_addr));

    return client_socket;
}

void execute_cmd_ls(int socketfd,char* current_path) {
    char buffer[1024];
    int buffer_len=0;
    memset(buffer,0,sizeof (buffer));
    // 目录流
    DIR *dirfd;
    struct dirent *ptr;
    dirfd = opendir(current_path);

    printf("目前查看的目录是%s\n",current_path);

    // 遍历该目录下的文件项
    while( (ptr = readdir(dirfd))!=NULL){
        if(!strcmp(ptr->d_name,".") || !strcmp(ptr->d_name,"..")) continue;
        printf("%s ",ptr->d_name);
        buffer_len += sprintf(buffer+buffer_len,"%s ",ptr->d_name);
    }
    printf("\n");
    send(socketfd,buffer, buffer_len+1,0);
    closedir(dirfd);
}

void execute_cmd_cd(int socketfd, char *current_path) {
    char newpath[128];
    int newpath_len=0;
    newpath_len = recv(socketfd,newpath,128,0);

    // 假设输入newpath开头总共就 `../文件名`   `./文件名`   `/文件名`   `文件名`4种情况
    if(newpath[0]=='.'&&newpath[1]=='.')
        dirname(current_path);
    else if(newpath[0] == '/')
        strcpy(current_path,newpath);
    else if(newpath[0] == '.')
        strcat(current_path,newpath+1);
    else {
        strcat(current_path,"/");
        strcat(current_path,newpath);
    }
    printf("cd命令修改后的current_path为%s\n",current_path);

    // 将新路径的信息传输回给客户端
    send(socketfd,current_path,128,0);
}

void execute_cmd_put(int socketfd) {
    // 接收客户端传输过来的文件名
    char filename[128];
    recv(socketfd,filename,128,0);

    // 检测文件所在目录是否存在
    char* dir;
    dir = strdup(filename);
    dirname(dir);
    if(access(dir,0)){
        printf("文件%s所在目录并不存在\n",dir);
        send(socketfd,  NET_MSG_ERROR, 3, 0);
        return;
    }
    send(socketfd,NET_MSG_OK,3,0);

    download(socketfd,filename);
}

void execute_cmd_get(int socketfd) {
    char filename[128];
    recv(socketfd,filename,128,0);

    // 检测文件是否存在
    if(access(filename,0)){
        printf("文件%s不存在\n",filename);
        send(socketfd,  NET_MSG_ERROR, 3, 0);
        return;
    }
    send(socketfd,NET_MSG_OK,3,0);

    upload(socketfd,filename);
}
