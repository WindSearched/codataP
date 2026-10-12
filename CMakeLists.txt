cmake_minimum_required(VERSION 4.3)
project(codataP)

set(CMAKE_CXX_STANDARD 26)

add_executable(codataP main.cpp
        codes/Tube.cpp
        codes/Tube.h
        codes/Event.cpp
        codes/Event.h
        codes/Branch.cpp
        codes/Branch.h
        codes/Core.cpp
        codes/Core.h)
