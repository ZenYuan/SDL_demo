#include <iostream>
using namespace std;
#define SDL_MAIN_HANDLED
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "SDL2/SDL.h"
}

int main() {
    //cout<<avutil_configuration()<<endl;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout<<"inti error\n"<<endl;
        return -1;
    }
    SDL_Rect rect;
    rect.x = 20;
    rect.y = 20;

    SDL_Window* window = SDL_CreateWindow("yuan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    //必须使用全路径
    SDL_Surface* image = SDL_LoadBMP("E:\\code\\c++_project\\ffmpeg_clion\\resources\\o.bmp");
    cout<<SDL_GetError()<<endl;

    SDL_UpperBlit(image, nullptr, surface, &rect);
    cout<<SDL_GetError()<<endl;

    //更新窗口
    SDL_UpdateWindowSurface(window);

    SDL_Delay(5000);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
