## 整体架构
项目分client,common,server三个目录
1. client: 构建ftp客户端，最后生成可执行文件ftp_client
2. server: 构建ftp服务器，最后生成可执行文件ftp_server
3. common: 该目录下包含，客户端和服务器可能共用的某些方法

## Usage
### Usage of ftp_client
To connect ftp server:
```shell
ftp_client [host] [port]

Description:
  host - 要连接的主机名或ip
  port - ftp_server监听的端口
```

Available commands:
```shell
ls - 显示当前目录下的文件
cd <dir> - 转移到另一个目录
put <local_file> <remote_file> - 上传文件
get <remote_file> <local_file> - 下载文件
quit - 退出
```