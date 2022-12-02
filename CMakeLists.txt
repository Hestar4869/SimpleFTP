cmake_minimum_required(VERSION 3.0)
project(SimpleFTP C)

set(CMAKE_C_STANDARD 99)

add_executable(SimpleFTP main.c)

add_executable(ftp_client client/ftp_client.c common/constant.h common/transfer_file.cpp common/transfer_file.h client/client_util.cpp client/client_util.h)