cmake_minimum_required(VERSION 3.0)
project(SimpleFTP)

set(CMAKE_CXX_STANDARD 11)


add_executable(ftp_client client/ftp_client.c common/constant.h common/transfer_file.c common/transfer_file.h client/client_util.c client/client_util.h common/constant.c)
add_executable(ftp_server server/ftp_server.cpp common/constant.c server/server_util.c server/server_util.h common/transfer_file.c common/transfer_file.h server/threadpool.cpp server/threadpool.h server/locker.h server/task.h)
target_link_libraries(ftp_server pthread)