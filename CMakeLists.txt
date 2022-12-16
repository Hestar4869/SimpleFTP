cmake_minimum_required(VERSION 3.0)
project(SimpleFTP C)

set(CMAKE_C_STANDARD 99)

add_executable(SimpleFTP main.c)

add_executable(ftp_client client/ftp_client.c common/constant.h common/transfer_file.c common/transfer_file.h client/client_util.c client/client_util.h common/constant.c)
add_executable(ftp_server server/ftp_server.c common/constant.c server/server_util.c server/server_util.h common/transfer_file.c common/transfer_file.h)
target_link_libraries(ftp_server pthread)