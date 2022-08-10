# 设置cmake的最低版本
cmake_minimum_required(VERSION 3.5)
# 设置工程名称 和版本
project(mods VERSION 0.1 LANGUAGES CXX)

# 指定为C++20 版本
set(CMAKE_CXX_STANDARD 20)
# 设置指定的C++编译器版本是必须的，如果不设置，或者为OFF，则指定版本不可用时，会使用上一版本。
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件
include_directories(include)

# 设置源码目录
aux_source_directory(src DIR_SRC)

# 设置静态库目录
link_directories(lib)

# 全静态编译，避免缺dll
set(CMAKE_EXE_LINKER_FLAGS "-static")

# 增加生成可执行文件
add_executable(${PROJECT_NAME} ${DIR_SRC})

# 为可执行程序链接库
target_link_libraries(${PROJECT_NAME} libjsoncpp.a)
