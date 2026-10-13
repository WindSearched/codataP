cmake_minimum_required(VERSION 4.3)
project(codataP)

set(CMAKE_CXX_STANDARD 26)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(codataP main.cpp
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
        codes/MainWindow.h)

target_link_libraries(codataP
        PRIVATE Qt6::Widgets
)