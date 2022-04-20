cmake_minimum_required(VERSION 3.22)
project(mixerCore)

set(CMAKE_CXX_STANDARD 20)

find_library(AVCODEC_LIBRARY avcodec)
find_library(AVFORMAT_LIBRARY avformat)
find_library(AVUTIL_LIBRARY avutil)
find_library(SWSCALE_LIBRARIES swscale)
find_library(THREADS_LIBRARIES pthreads)


include_directories(
        external/ffmpeg
        /usr/local/include/
        .
)
set(ALL_LIBS
        ${THREADS_LIBRARIES},
        ${AVCODEC_LIBRARY}
        ${AVFORMAT_LIBRARY}
        ${AVUTIL_LIBRARY}
        ${SWSCALE_LIBRARIES}

        )


add_executable(mixerCore main.cpp CRender.cpp CRender.h)
