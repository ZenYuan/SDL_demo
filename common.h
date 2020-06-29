//
// Created by ZenYuan on 2020/6/26.
//

#ifndef FFMPEG_CLION_COMMON_H
#define FFMPEG_CLION_COMMON_H

#include <iostream>
#include <string>
#include <cstdio>
using namespace std;
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/mathematics.h"
//#include "SDL2/SDL.h"
//#include "SDL2/SDL_render.h"
}

int decodec_test();
int code_test();
int Transcod_test();
//int display();

#endif //FFMPEG_CLION_COMMON_H
