//
// Created by Hestar on 2022/12/2.
//
#include <stdio.h>
#include <stdlib.h>
#include "ftp_client.h"

int main(int argc,char *argv[]){
    char* host ;
    int port;
    // �ͻ�������ʱ����ftp [host] [port]
    if(argc == 3){
        host = argv[1];
        port = atoi(argv[2]);
        printf("������������%s:%d......",host,port);
    }
    else{
        sprintf(stderr,"���������Ŀ����");
        exit(-1);
    }

    // ����socket������

}