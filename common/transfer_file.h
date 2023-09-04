//
// Created by Hestar on 2022/12/2.
// 传输文件时所用到的相关方法
//

#ifndef SIMPLEFTP_TRANSFER_FILE_H
#define SIMPLEFTP_TRANSFER_FILE_H

// 从本地传输到远端socket
void upload(int socketfd,char* filename);

// 从socket中下载到本地
void download(int socketfd,char* filename);
#endif //SIMPLEFTP_TRANSFER_FILE_H
