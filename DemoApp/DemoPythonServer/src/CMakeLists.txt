
cmake_minimum_required(VERSION 2.8)

project(DemoApp-DemoPythonServer)

gen_server(DemoApp DemoPythonServer)

#target_link_libraries(mysqlclient ssl crypto nghttp2-static)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})

aux_source_directory(. DIR_SRC)

target_link_libraries(DemoPythonServer ${Python3_LIBRARIES})