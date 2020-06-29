cmake_minimum_required(VERSION 3.16)
project(ffmpeg_clion)
set(CMAKE_CXX_STANDARD 20)

include_directories(./ffmpeg/include)
link_directories(./ffmpeg/lib)
include_directories(./sdl/include)
link_directories(./sdl/lib)

link_libraries(avcodec)
link_libraries(avdevice)
link_libraries(avfilter)
link_libraries(avformat)
link_libraries(avutil)
link_libraries(postproc)
link_libraries(swresample)
link_libraries(swscale)

link_libraries(mingw32)
#link_libraries(SDL2)
#link_libraries(SDL2main)



add_executable(ffmpeg_clion main.cpp decodec.cpp common.h SDL.cpp codec.cpp tanscodec.cpp)