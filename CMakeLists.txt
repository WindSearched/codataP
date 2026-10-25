cmake_minimum_required(VERSION 4.3)
project(codataP)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC  ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)
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
        codes/MainWindow.h
        codes/classes/TubeCache.h
        codes/branches/math.h
        utils/SingleInstance.cpp
        utils/SingleInstance.h
        codes/branches/math.cpp
)

target_link_libraries(codataP PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
)

add_subdirectory(CTerminal)

if(WIN32)
    set_target_properties(Codata PROPERTIES
            WIN32_EXECUTABLE FALSE
            LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()