cmake_minimum_required(VERSION 3.22)
project(mixerCore)

set(CMAKE_CXX_STANDARD 20)


# Find where Magick++-config lives
find_program(MAGICK_CONFIG "Magick++-config")
# Ask about CXX and lib flags/locations
execute_process(COMMAND "${MAGICK_CONFIG}" "--cxxflags" OUTPUT_VARIABLE MAGICK_CXX_FLAGS)
execute_process(COMMAND "${MAGICK_CONFIG}" "--libs" OUTPUT_VARIABLE MAGICK_LD_FLAGS)
string(STRIP "${MAGICK_CXX_FLAGS}" MAGICK_CXX_FLAGS)
string(STRIP "${MAGICK_LD_FLAGS}" MAGICK_LD_FLAGS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${MAGICK_CXX_FLAGS} ${MAGICK_LD_FLAGS}")




find_library(AVCODEC_LIBRARY avcodec)
find_library(AVFORMAT_LIBRARY avformat)
find_library(AVUTIL_LIBRARY avutil)
find_library(SWSCALE_LIBRARIES swscale)
find_library(IM_LIBRARIES Magick++)

include_directories(

        external/ffmpeg
        /usr/include/x86_64-linux-gnu/
        /usr/local/include/
        /usr/include/
        ${ImageMagick_INCLUDE_DIRS}
        .
)

set(ALL_LIBS
        ${AVCODEC_LIBRARY}
        ${AVFORMAT_LIBRARY}
        ${AVUTIL_LIBRARY}
        ${SWSCALE_LIBRARIES}
        ${CONFIG_LIBRARIES}
        ${IM_LIBRARIES}
        )

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
        -DMAGICKCORE_QUANTUM_DEPTH=16
        -DMAGICKCORE_HDRI_ENABLE=0
)


set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0" CACHE STRING "")


add_executable(mixerCore main.cpp CHttp.cpp CHttp.h CConfig.cpp CConfig.h Ccmd.cpp Ccmd.h CFFreader.cpp CFFreader.h CffmpegStreamer.cpp CffmpegStreamer.h SstreamData.cpp SstreamData.h CEvent.cpp CEvent.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries( mixerCore PRIVATE Threads::Threads ${ALL_LIBS} ${ImageMagick_LIBRARIES})