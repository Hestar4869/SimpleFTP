//
// Created by Hestar on 2023/9/4.
//

#ifndef SIMPLEFTP_TASK_H
#define SIMPLEFTP_TASK_H

// 为适配线程池正常运行，必须实现process()
class Task{
public:
    Task(int sockfd):m_sockfd(sockfd){
        // 初始化该线程的工作路径
        memset(current_path,0,sizeof (current_path));
        strcpy(current_path,"/home/hestar");
    }

    void process(){
        char cmd_buffer[5];
        int num= recv(m_sockfd, cmd_buffer, 5, 0);
        printf("收到%d个字节，命令%s\n",num,cmd_buffer);
        switch (str_to_cmd(cmd_buffer)) {
            case CMD_LS:
                execute_cmd_ls(m_sockfd, current_path);
                break;
            case CMD_CD:
                execute_cmd_cd(m_sockfd, current_path);
                break;
            case CMD_PUT:
                execute_cmd_put(m_sockfd);
                break;
            case CMD_GET:
                execute_cmd_get(m_sockfd);
                break;
            case CMD_QUIT:
                //表示可以退出该线程
                close(m_sockfd);
        }

    }
private:
    int m_sockfd;
    char current_path[128];
};


#endif //SIMPLEFTP_TASK_H
