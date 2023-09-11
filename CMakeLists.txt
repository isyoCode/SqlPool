cmake_minimum_required(VERSION 3.10)
project(SqlPool)


set(CMKAE_CXX_STANDARD 11)

#设置文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)



#包含执行文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(/usr/include/mysql)

add_subdirectory(src)

#install_files()

#先制作动态链接库
#add_library()
