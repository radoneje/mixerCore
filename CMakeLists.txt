cmake_minimum_required(VERSION 3.22)
project(mixerCore)

set(CMAKE_CXX_STANDARD 20)


include_directories(
        external/ffmpeg
        /usr/local/include/
        .
)


add_executable(mixerCore main.cpp CRender.cpp CRender.h)
