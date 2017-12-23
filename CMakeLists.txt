cmake_minimum_required(VERSION 3.9)
project(j5c_install_from_source_001)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        main.cpp
        )

add_executable(Install ${SOURCE_FILES})