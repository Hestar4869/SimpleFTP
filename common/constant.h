//
// Created by Hestar on 2022/12/2.
// 用于定义一些全局常量
//

#ifndef SIMPLEFTP_CONSTANT_H
#define SIMPLEFTP_CONSTANT_H

// 网络传输中，信息是否错误
#define NET_MSG_ERROR "NO"
#define NET_MSG_OK "OK"

// 枚举命令类型
enum COMMOND{
    CMD_LS=0,CMD_CD,CMD_PUT,CMD_GET,CMD_QUIT
};
// COMMAND枚举类型转成对应的控制台输入的字符串命令
char* cmd_to_str(enum COMMOND cmd);
// 控制台输入的字符串命令转成对应的COMMAND枚举类型
enum COMMOND str_to_cmd(char* str);
#endif //SIMPLEFTP_CONSTANT_H
