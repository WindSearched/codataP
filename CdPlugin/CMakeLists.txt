cmake_minimum_required(VERSION 3.20)
project(CodataPI)

# 编译成静态库 (STATIC) 或 动态库 (SHARED)
add_library(CodataPI STATIC
        CdaPI.cpp
        Center.cpp
        Center.h
        Center.cpp
        CdaPI.h
)

# 配置头文件搜索路径
# PUBLIC 关键字非常关键：
# 1. 编译自己时：要用到 include/ 目录
# 2. 别人引用你时：别人也会自动把 include/ 目录加入搜索路径
target_include_directories(CodataPI PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
