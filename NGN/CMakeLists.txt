cmake_minimum_required(VERSION 2.8)
project(NGN)

add_subdirectory(ngn_interface)
add_subdirectory(broker)
add_subdirectory(debug_cli)
add_subdirectory(msgpk_tools)
add_subdirectory(worker)
#add_subdirectory(client)
add_subdirectory(version)


