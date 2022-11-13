# 设置cmake的最低版本
cmake_minimum_required(VERSION 3.5)
# 设置Release构建类型，更小构建体积
# set(CMAKE_BUILD_TYPE "Release")
# MingW: 全静态编译，避免缺dll
set(CMAKE_CXX_FLAGS "-static")

# 设置 vcpkg
set(CMAKE_TOOLCHAIN_FILE "/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "" FORCE)

# 设置工程名称 和版本
project(file-sync VERSION 0.1 LANGUAGES CXX)
# 指定为C++20 版本
set(CMAKE_CXX_STANDARD 20)
# 设置指定的C++编译器版本是必须的，如果不设置，或者为OFF，则指定版本不可用时，会使用上一版本。
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 导入库
find_package(OpenSSL REQUIRED)

# 包含头文件
include_directories(${PROJECT_SOURCE_DIR}/include)
# 设置源码目录
aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRC)
# 设置静态库目录
link_directories(${PROJECT_SOURCE_DIR}/lib)
# 增加生成可执行文件
add_executable(${PROJECT_NAME} ${DIR_SRC})

# 链接库到可执行程序
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# 设置编译文件的目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
