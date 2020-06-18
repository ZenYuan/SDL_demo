#include <iostream>
#include <string>
using namespace std;
#define SDL_MAIN_HANDLED
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
}

char *file_path = "";

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_TOTAL
};

SDL_Window *gWindow = nullptr;
SDL_Surface *gScreenSurface = nullptr;
SDL_Event e;
SDL_Surface *gKeyPressSurface[KEY_PRESS_SURFACE_TOTAL] = {0};
SDL_Surface *gCurrSurface = nullptr;

SDL_Window *gWindowR = nullptr;
SDL_Renderer *gRenderer = nullptr;
SDL_Texture *gTexture = nullptr;

bool init()
{
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout<<SDL_GetError()<<endl;
        success = false;
    }
    else
    {
        //gWindow = SDL_CreateWindow("yuan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
        gWindowR = SDL_CreateWindow("SDL_Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
        if(nullptr == gWindow && nullptr == gWindowR)
        {
            cout<<SDL_GetError()<<endl;
            success = false;
        }
        else
        {
            gScreenSurface = SDL_GetWindowSurface(gWindowR);

            //create renderer for window
            gRenderer = SDL_CreateRenderer(gWindowR, -1, SDL_RENDERER_ACCELERATED);
            if(nullptr == gRenderer)
            {
                cout<<SDL_GetError()<<endl;
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
            }
        }
    }
    return success;
}

SDL_Surface* loadSurface(const string& path)
{
    SDL_Surface *optimizedSurface = nullptr;
    char* path_cstr = const_cast<char*>(path.c_str());
    SDL_Surface* tmpSurface = SDL_LoadBMP(path_cstr);
    if(nullptr == tmpSurface)
    {
        cout<<SDL_GetError()<<endl;
        return nullptr;
    }
    else
    {
        //Convert surface to screen surface to avoid per time
        optimizedSurface = SDL_ConvertSurface(tmpSurface, gScreenSurface->format, 0);
        SDL_FreeSurface(tmpSurface);
    }
    return optimizedSurface;
}

bool loadMedia()
{
    bool success = true;

    gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("F:\\Project\\Project_clion\\SDL_demo\\resources\\dog.bmp");
    if(nullptr == gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT])
    {
        success = false;
    }
    gKeyPressSurface[KEY_PRESS_SURFACE_UP] = loadSurface("F:\\Project\\Project_clion\\SDL_demo\\resources\\o.bmp");
    if(nullptr == gKeyPressSurface[KEY_PRESS_SURFACE_UP])
    {
        success = false;
    }
    gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] = loadSurface("F:\\Project\\Project_clion\\SDL_demo\\resources\\sky.bmp");
    if(nullptr == gKeyPressSurface[KEY_PRESS_SURFACE_DOWN])
    {
        success = false;
    }
    return success;
}

void close()
{
    //Deallocte surface
    for(int i =0 ; i < KEY_PRESS_SURFACE_TOTAL; i++)
    {
        SDL_FreeSurface(gKeyPressSurface[i]);
        gKeyPressSurface[i] = nullptr;
    }
    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = nullptr;

    //Dsetory window
    SDL_DestroyWindow(gWindowR);
    gWindowR = nullptr;

    SDL_DestroyTexture(gTexture);
    gTexture = nullptr;
    //Quit SDL subSystem
    SDL_Quit();
}

int display()
{
    bool quite = false;
    if(!init())
    {
        cout<<"init error\n"<<endl;
        return 0;
    }
    else
    {
        if(!loadMedia())
        {
            cout<<"loadMedia error\n"<<endl;
            return 0;
        }
        else
        {
            SDL_Rect rect;
            rect.x = 0;
            rect.y = 0;

            while(!quite)
            {
                //Handle
                while(SDL_PollEvent(&e) != 0)
                {
                    if(e.type == SDL_QUIT)
                    {
                        quite = true;
                    }
                    else if(e.type = SDL_KEYDOWN)
                    {
                        switch(e.key.keysym.sym)
                        {
                            case SDLK_UP:
                                gCurrSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
                                break;
                            case SDLK_DOWN:
                                gCurrSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DOWN];
                                break;
                            default:
                                gCurrSurface = gKeyPressSurface[KEY_PRESS_SURFACE_UP];
                        }
                    }
                }
                gTexture = SDL_CreateTextureFromSurface(gRenderer, gCurrSurface);
                //clear screen
                SDL_RenderClear(gRenderer);
                //Render texture to screen
                SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);
                //Update screen
                SDL_RenderPresent(gRenderer);

//                Apply image
//                SDL_BlitScaled(gCurrSurface, nullptr, gScreenSurface, nullptr);
//                Update image
//                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }
    close();
}


int main(int argc, char* argv[]) {
    AVFormatContext *pFormatContext = nullptr;
    //read file header of file format
    if(avformat_open_input(&pFormatContext, file_path, nullptr, nullptr) < 0){
        cout<<"avformat_open_input error\n"<<endl;
        return -1;
    }

    //get stream from format
    if(avformat_find_stream_info(pFormatContext, nullptr) < 0){
        cout<<"avformat_find_stream_info"<<endl;
        return -1;
    }

    int videoIndex = -1;
    for(int i = 0; i < pFormatContext->nb_streams; i++){
        if(pFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            break;
        }
    }
    if(-1 == videoIndex){
        cout<<"not a video stream\n"<<endl;
        return -1;
    }

    AVCodecParameters *pCodecParameters = pFormatContext->streams[videoIndex]->codecpar;
    //根据id值注册一个解码器。
    AVCodec *pCodec = avcodec_find_decoder(pCodecParameters->codec_id);
    //初始化解码器参数,将文件数据解码器参数拷贝
    AVCodecContext *pCodecContext = avcodec_alloc_context3(pCodec);
    if(nullptr == pCodecContext){
        cout<<"pCodecContext error\n"<<endl;
        return -1;
    }
    avcodec_parameters_to_context(pCodecContext, pCodecParameters);
    //打开编码器，此处预先解码头信息
    if(avcodec_open2(pCodecContext, pCodec, nullptr) < 0){
        cout<<"avcodec_open2"<<endl;
        return -1;
    }

    AVFrame *pFrame = av_frame_alloc();
    AVPacket *pPacket = static_cast<AVPacket*>(av_malloc(sizeof(AVPacket)));

    cout<<"#####################file Infomation#############\n"<<endl;
    av_dump_format(pFormatContext, videoIndex, file_path, 0);
    cout<<"#####################end #######################\n"<<endl;

    while(av_read_frame(pFormatContext, pPacket) >= 0){
        if(pPacket->stream_index == videoIndex){
            if(0 != avcodec_send_packet(pCodecContext, pPacket)){
                cout<<"send packet data to codecContext error\n"<<endl;
                return -1;
            }
            while(1){
                int res = avcodec_receive_frame(pCodecContext, pFrame);
                if(AVERROR(EAGAIN) == res || AVERROR_EOF == res){
                    break;
                }
                else if(res < 0){
                    cout<<"reveice a fream  from pCodecContext error\n"<<endl;
                    return -1;
                }
                if(res >= 0){
                    //sws_scale()
                }
            }

        }

    }





    return 0;
}
