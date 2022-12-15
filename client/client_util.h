//
// Created by Hestar on 2022/12/2.
//

#ifndef SIMPLEFTP_CLIENT_UTIL_H
#define SIMPLEFTP_CLIENT_UTIL_H
#include "../common/constant.h"
// socket相关方法
int connect_server(int socketfd, char* host , int port);

// 传输控制信息的方法
int send_cmd_ls(int socketfd);
int send_cmd_cd(int socketfd);
int send_cmd_put(int socketfd);
int send_cmd_get(int socketfd);
int send_cmd_quit(int socketfd);
#endif //SIMPLEFTP_CLIENT_UTIL_H
