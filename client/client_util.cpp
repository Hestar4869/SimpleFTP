//
// Created by Hestar on 2022/12/2.
//

#include "client_util.h"

int send_cmd_ls(...) {
    //接受传送过来的socket参数,发送CMD_LS宏信息
    // code here

    //等待响应ACK码
    // code here

    //收到ls命令信息

    //打印控制台
    return 0;
}

int send_cmd_cd(...) {
    //利用传送过来的socket参数,发送CMD_CD宏信息
    // code here

    //等待响应ACK码
    // code here

    //收到cd命令信息

    //打印控制台
    return 0;
}

int send_cmd_put(...) {
    //利用传递的socket参数 ,local_file ,remote_file，发送CMD_PUT宏信息

    //等待响应ACK码

    //开启新线程并创建新socket连接，传输端口

    //利用common/transfer.h中方法，发送文件

    //等待结束，并打印信息
    return 0;
}

int send_cmd_get(...) {
    //与 send_cmd_put() 类似
    return 0;
}

int send_cmd_quit(...) {
    // 发送CMD_QUIT

    //等待响应

    //原地释放资源 ， 或函数返回后释放
    return 0;
}


