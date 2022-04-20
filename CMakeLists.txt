cmake_minimum_required(VERSION 3.22)
project(mixerCore)

set(CMAKE_CXX_STANDARD 20)



find_library(AVCODEC_LIBRARY avcodec)
find_library(AVFORMAT_LIBRARY avformat)
find_library(AVUTIL_LIBRARY avutil)
find_library(SWSCALE_LIBRARIES swscale)
find_library(GLUT_LIBRARIES glut)


include_directories(
        external/ffmpeg
        /usr/include
        .
)
set(ALL_LIBS
        ${AVCODEC_LIBRARY}
        ${AVFORMAT_LIBRARY}
        ${AVUTIL_LIBRARY}
        ${SWSCALE_LIBRARIES}
        ${GLUT_LIBRARIES}
        )


add_executable(mixerCore main.cpp CRender.cpp CRender.h CTexture.cpp CTexture.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(mixerCore PRIVATE Threads::Threads)