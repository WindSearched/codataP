cmake_minimum_required(VERSION 4.3)
project(codataP)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC  ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)
find_package(yaml-cpp REQUIRED)
find_package(sol2 REQUIRED)
find_package(Lua REQUIRED)
find_package(CURL REQUIRED)

## 1. 查找系统 Lua (推荐 5.1 - 5.4 版本)
#find_package(Lua REQUIRED)
#
## 2. 使用 FetchContent 引入 sol2
#include(FetchContent)
#FetchContent_Declare(
#        sol2
#        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
#        GIT_TAG v4.0.0 # 建议指定一个稳定的 tag
#)
#FetchContent_MakeAvailable(sol2)


qt_standard_project_setup()

add_executable(codataP WIN32
        main.cpp
        codes/Tube.cpp
        codes/Tube.h
        codes/Event.cpp
        codes/Event.h
        codes/Branch.cpp
        codes/Branch.h
        codes/Core.cpp
        codes/Core.h
        codes/SEvent.cpp
        codes/SEvent.h
        codes/MainWindow.cpp
        codes/MainWindow.h
        codes/classes/TubeCache.h
        utils/SingleInstance.cpp
        utils/SingleInstance.h
        codes/branches/math.cpp
        codes/branches/math.h
        codes/Data.cpp
        codes/Data.h
        codes/Sets.cpp
        codes/Sets.h
        codes/classes/Result.cpp
        codes/classes/Result.h
        codes/classes/BranchBuilder.cpp
        codes/classes/BranchBuilder.h
        codes/plugins/lua/LuaMan.cpp
        codes/plugins/lua/LuaMan.h
        codes/classes/CdFunc.cpp
        codes/classes/CdFunc.h
        codes/classes/CommandArgument.cpp
        codes/classes/CommandArgument.h
        codes/plugins/executable/Exec.cpp
        codes/plugins/executable/Exec.h
        codes/plugins/lua/LuaAPI.cpp
        codes/plugins/lua/LuaAPI.h
        codes/plugins/PluginsMan.cpp
        codes/plugins/PluginsMan.h
)
target_include_directories(codataP PRIVATE ${LUA_INCLUDE_DIR})

target_link_libraries(codataP PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        yaml-cpp::yaml-cpp
        sol2::sol2 ${LUA_LIBRARIES}
        CURL::libcurl
)

if(WIN32)
    # 只要将 WIN32_EXECUTABLE 设为 FALSE，MinGW 就会默认保留控制台窗口
    set_target_properties(codataP PROPERTIES WIN32_EXECUTABLE TRUE)
endif()