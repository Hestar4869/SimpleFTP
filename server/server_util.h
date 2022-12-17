//
// Created by Hestar on 2022/12/16.
//

#ifndef SIMPLEFTP_SERVER_UTIL_H
#define SIMPLEFTP_SERVER_UTIL_H

#define PORT 66
// 创建用于监听端口的socket
int create_listen_server();

// 输入监听socket，等待一个socket接入并返回这个socket
int wait_client(int listen_socket);

// 处理控制信息的方法
void execute_cmd_ls(int socketfd,char* current_path);
void execute_cmd_cd(int socketfd,char* current_path);
void execute_cmd_put(int socketfd);
void execute_cmd_get(int socketfd);
#endif //SIMPLEFTP_SERVER_UTIL_H
