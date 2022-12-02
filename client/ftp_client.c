//
// Created by Hestar on 2022/12/2.
//
#include <stdio.h>
#include <stdlib.h>
#include "ftp_client.h"

int main(int argc,char *argv[]){
    char* host ;
    int port;
    // 客户端连接时，用ftp [host] [port]
    if(argc == 3){
        host = argv[1];
        port = atoi(argv[2]);
        printf("正在连接主机%s:%d......",host,port);
    }
    else{
        sprintf(stderr,"输入参数数目错误");
        exit(-1);
    }

    // 创建socket并连接

}