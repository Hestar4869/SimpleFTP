#!/bin/bash
## 该脚本用于构建一个临时的tcp服务器，监听66端口，用于测试客户端控制信息传递功能
for ((;;))
do
   output=`nc -l 66|tr -d "\1"`
    echo $output
done
