cmake_minimum_required(VERSION 3.22)
project(mixerCore)

set(CMAKE_CXX_STANDARD 20)



find_library(AVCODEC_LIBRARY avcodec)
find_library(AVFORMAT_LIBRARY avformat)
find_library(AVUTIL_LIBRARY avutil)
find_library(SWSCALE_LIBRARIES swscale)
find_library(GLUT_LIBRARIES glut)


include_directories(
        external/AntTweakBar-1.16/include/
        external/glfw-3.1.2/include/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        external/assimp-3.0.1270/include/
        external/bullet-2.81-rev2613/src/
        external/ffmpeg
        /usr/include/x86_64-linux-gnu/
        /usr/local/include/
        .
)
set(ALL_LIBS
        ${AVCODEC_LIBRARY}
        ${AVFORMAT_LIBRARY}
        ${AVUTIL_LIBRARY}
        ${SWSCALE_LIBRARIES}
        ${GLUT_LIBRARIES}
        glfw
        GLEW_1130
        )
add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)




add_executable(mixerCore main.cpp CRender.cpp CRender.h CTexture.cpp CTexture.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries( mixerCore PRIVATE Threads::Threads ${ALL_LIBS})