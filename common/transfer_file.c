//
// Created by Hestar on 2022/12/2.
//

#include <bits/types/FILE.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "transfer_file.h"

void upload(int socketfd, char *filename) {
    FILE *fp;
    fp = fopen(filename,"rb");

    char buffer[1024];
    int file_size=0 , len=0;
    while((len = fread(buffer,sizeof(char),1024,fp))>0){
        file_size += len;
        send(socketfd,buffer,len,0);

    }
    printf("总共上传%d字节，约%dMb\n",file_size,file_size/1024/1024);

    fclose(fp);
}

void download(int socketfd, char *filename) {
    FILE *fp;
    fp = fopen(filename,"wb");

    char buffer[1024];
    int file_size=0,len=0;
    while ((len = recv(socketfd,buffer,sizeof (buffer),0))>0){
        file_size += len;
        fwrite(buffer,sizeof (buffer),len,fp);

        // recv()函数是阻塞式的，如果len不是1024则说明已经读取完毕
        if(len != 1024) break;
    }
    printf("总共下载文件%d字节，约%dMB\n",file_size,file_size/1024/1024);
    fclose(fp);
}
