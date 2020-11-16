cmake_minimum_required(VERSION 3.11)
project(j5c_install_from_source_001)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
        source/j5c_date.h
        source/j5c_date.cpp
        main.cpp
        )

add_executable(Install.sh ${SOURCE_FILES})

